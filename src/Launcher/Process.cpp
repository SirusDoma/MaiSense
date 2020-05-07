#include <MaiSense/Launcher/Process.hpp>

namespace MaiSense
{
    
    Process::Process(std::string moduleName) :
        moduleName(moduleName),
        processId(0),
        threadId(0),
        hProcess(0),
        hThread(0),
        hModule(0)
    {
        if (!moduleName.empty())
            GetProcessHandle();
    }

    Process::Process(HANDLE hProcess, HANDLE hThread, int processId, int threadId) :
        moduleName(),
        processId(processId),
        threadId(threadId),
        hProcess(hProcess),
        hThread(hThread),
        hModule(0)
    {
    }

    Process::~Process()
    {
        if (hProcess)
            CloseHandle(hProcess);

        if (hThread)
            CloseHandle(hThread);
    }

    Process Process::Create(std::string path, bool suspended)
    {
        return Create(path, "", suspended);
    }

    Process Process::Create(std::string path, std::string args, bool suspended)
    {
        STARTUPINFO si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        bool success = CreateProcess(
            path.c_str(),
            const_cast<char*>(args.c_str()),
            NULL,
            NULL,
            TRUE,
            suspended ? CREATE_SUSPENDED : 0,
            NULL,
            NULL,
            &si,
            &pi
        );

        if (!success)
        {
            std::fprintf(stderr, "MAISENSE: Failed to create process\n");
            return Process("");
        }

        return Process(pi.hProcess, pi.hThread, pi.dwProcessId, pi.dwThreadId);
    }

    DWORD Process::GetTargetAddress(DWORD address, bool relative)
    {
        DWORD target = address;
        if (relative)
            target = (DWORD)GetBaseAddress() + address;

        return target;
    }

    HANDLE Process::GetProcessHandle()
    {
        // Check for existing process
        if (hProcess)
            return hProcess;

        // Create snapshots of processes
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);

        // Get Process from processes snapshot
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        if (Process32First(snapshot, &entry) == TRUE)
        {
            while (Process32Next(snapshot, &entry) == TRUE)
            {
                // Check for target process that match with moduleName
                std::string exeFile = entry.szExeFile;
                if (exeFile.find(moduleName) != std::string::npos)
                {
                    // Process found, attempt to open process
                    processId = entry.th32ProcessID;
                    hProcess = OpenProcess(
                        PROCESS_ALL_ACCESS,
                        FALSE,
                        entry.th32ProcessID
                    );

                    break;
                }
            }
        }

        // Check process handle
        if (!hProcess)
            std::fprintf(stderr, "MAISENSE: Failed to open process\n");

        return hProcess;
    }

    HANDLE Process::GetThreadHandle()
    {
        // Check for existing thread
        if (hThread)
            return hThread;

        // Check for existing process
        if (!processId)
            GetProcessHandle();

        // Look after thread handle
        THREADENTRY32 tEntry;
        tEntry.dwSize = sizeof(THREADENTRY32);
        auto hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

        // Validate snapshot
        if (hThreadSnapshot == INVALID_HANDLE_VALUE)
        {
            std::fprintf(stderr, "MAISENSE: Failed to open find thread\n");
            return hProcess;
        }

        // Get Thread from threads snapshot
        if (Thread32First(hThreadSnapshot, &tEntry) == TRUE)
        {
            while (Thread32Next(hThreadSnapshot, &tEntry) == TRUE)
            {
                // Check for thread process id that match with handle process
                if (tEntry.th32OwnerProcessID == processId)
                {
                    threadId = tEntry.th32ThreadID;
                    hThread  = OpenThread(
                        THREAD_ALL_ACCESS,
                        FALSE,
                        tEntry.th32ThreadID
                    );

                    break;
                }
            }
        }

        // Check process handle
        if (!hThread)
            std::fprintf(stderr, "MAISENSE: Failed to locate thread handle\n");

        return hThread;
    }

    int Process::GetProcessId()
    {
        if (!processId)
            GetProcessHandle();

        return processId;
    }

    int Process::GetThreadId()
    {
        if (!threadId)
            GetThreadHandle();

        return threadId;
    }

    LPCVOID Process::GetBaseAddress()
    {
        // Retrieve process handle if not yet fetched
        if (!hProcess)
            GetProcessHandle();

        // Return existing address if previous query handle still valid
        if (hModule)
            return hModule;

        // Define variables to query modules
        HMODULE hMods[1024];
        DWORD cbNeeded;
        LPCVOID address = 0;

        // Enumerate the list of loaded module in the process handle
        if (hProcess && EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
        {
            for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
            {
                // Resolve module filename
                TCHAR szModName[MAX_PATH];
                if (GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
                {
                    // Check if module matched with target module name
                    std::string name = szModName;
                    if (name.find(moduleName) != std::string::npos)
                    {
                        // Module found, get the address
                        address = hMods[i];
                        break;
                    }
                }
            }
        }

        // No main module found
        if (!address)
            std::fprintf(stderr, "MAISENSE: Failed to find module.\n");

        hModule = address;
        return address;
    }


    bool Process::Resume()
    {
        if (!hThread)
        {
            std::fprintf(stderr, "MAISENSE: No valid thread handle found\n");
            return false;
        }

        if (!ResumeThread(hThread))
        {
            std::fprintf(stderr, "MAISENSE: Failed to resume thread\n");
            return false;
        }

        return true;
    }

    bool Process::Suspend()
    {
        if (!hThread)
        {
            std::fprintf(stderr, "MAISENSE: No valid thread handle found\n");
            return false;
        }

        if (!SuspendThread(hThread))
        {
            std::fprintf(stderr, "MAISENSE: Failed to suspend thread\n");
            return false;
        }

        return true;
    }

    void Process::Wait()
    {
        WaitForSingleObject(hProcess, INFINITE);
    }

    unsigned int Process::Read(DWORD address, bool relative)
    {
        // Validate process handle
        if (!hProcess)
        {
            std::fprintf(stderr, "MAISENSE: Invalid process handle.\n");
            return 0;
        }

        // Calculate address
        LPCVOID target = (LPCVOID)GetTargetAddress(address, relative);

        // Read data from target address process memory
        unsigned int result = 0;
        if (!ReadProcessMemory(hProcess, target, &result, sizeof(result), NULL))
            std::fprintf(stderr, "MAISENSE: Failed to read process memory.\n");

        return result;
    }

    unsigned int Process::Write(DWORD address, LPCVOID buffer, SIZE_T size, bool relative)
    {
        // Validate process handle
        if (!hProcess)
        {
            std::fprintf(stderr, "MAISENSE: Invalid process handle.\n");
            return 0;
        }

        // Calculate address
        LPVOID target = (LPVOID)GetTargetAddress(address, relative);

        // Write data to target address process memory
        SIZE_T bytesWritten = 0;
        if (!WriteProcessMemory(hProcess, target, buffer, size, &bytesWritten))
            std::fprintf(stderr, "MAISENSE: Failed to write process memory %d.\n", GetLastError());

        return bytesWritten;
    }
}