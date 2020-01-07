#include <MaiSense/Launcher/Injector.hpp>
#include <Windows.h>

namespace MaiSense::Launcher
{
    Injector::Injector(MaiSense::Process *process) :
        process(process),
        remoteAddr(0),
        remoteThread(0)
    {
    }

    HANDLE Injector::Inject(std::string dll_name)
    {
        // Lookup DLL to inject
        if (!SearchPath(NULL, dll_name.c_str(), NULL, 0, NULL, NULL))
        {
            std::fprintf(stderr, "MAISENSE: DLL to inject not found (%s).\n", dll_name.c_str());
            return 0;
        }

        // Allocate memory to inject the DLL
        remoteAddr = VirtualAllocEx(
            process->GetProcessHandle(),
            NULL,
            dll_name.length() + 1,
            MEM_RESERVE | MEM_COMMIT,
            PAGE_READWRITE
        );

        // Check allocation operation
        if (!remoteAddr)
        {
            std::fprintf(stderr, "MAISENSE: Failed to allocate DLL memory (%d).\n", GetLastError());
            return 0;
        }

        // Write dll name into allocated memory
        bool success = WriteProcessMemory(
            process->GetProcessHandle(),
            remoteAddr,
            dll_name.c_str(),
            dll_name.length() + 1,
            NULL
        );

        // Check write DLL operation
        if (!success) 
        {
            std::fprintf(stderr, "MAISENSE: Failed to write DLL into memory (%d).\n", GetLastError());
            Close();

            return 0;
        }

        // Create remote thread to load dll that written into memory
        remoteThread = CreateRemoteThread(
            process->GetProcessHandle(),
            NULL,
            0,
            (LPTHREAD_START_ROUTINE)LoadLibraryA,
            remoteAddr,
            0,
            NULL
        );

        // Validate remote thread
        if (!remoteThread) 
        {
            std::fprintf(stderr, "MAISENSE: Failed to CreateRemoteThread (%d).\n", GetLastError());
            Close();

            return remoteThread;
        }

        if (WaitForSingleObject(remoteThread, INFINITE) != WAIT_OBJECT_0)
            std::fprintf(stderr, "MAISENSE: Failed to wait object (%d).\n", GetLastError());

        DWORD result;
        if (!GetExitCodeThread(remoteThread, &result))
            std::fprintf(stderr, "MAISENSE: Failed to wait exit thread (%d).\n", GetLastError());

        if (result == 0)
            std::fprintf(stderr, "MAISENSE: Failed to load inside target process (%d).\n", GetLastError());

        // return created remote thread
        return remoteThread;
    }

    bool Injector::Close()
    {
        // Dispose remote thread
        if (remoteThread != NULL)
            CloseHandle(remoteThread);

        // Free allocated memory for DLL
        if (remoteAddr) 
        {
            bool success = VirtualFreeEx(process->GetProcessHandle(), remoteAddr, 0, MEM_RELEASE);
            if (!success)
            {
                std::fprintf(stderr, "MAISENSE: Failed to clear allocated dll memory (%d).\n", GetLastError());
                return false;
            }
        }

        return true;
    }
}