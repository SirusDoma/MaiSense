#include <MaiSense/Process.hpp>

namespace MaiSense
{
    
    Process::Process(std::string moduleName) :
        moduleName(moduleName),
        hProcess(0),
        baseAddress(0)
    {
        GetProcessHandle();
    }

    Process::~Process()
    {
        if (hProcess)
            CloseHandle(hProcess);
    }

    DWORD Process::GetTargetAddress(DWORD address, bool relative)
    {
        DWORD target = address;
        if (relative)
            target = (DWORD)baseAddress + address;

        return target;
    }

    HANDLE* Process::GetProcessHandle()
    {
        baseAddress = GetBaseAddress();
        return &hProcess;
    }

    LPCVOID Process::GetBaseAddress()
    {
        // Return existing address if previous query handle still valid
        if (hProcess && baseAddress)
            return baseAddress;

        // Define variables to query modules
        PROCESSENTRY32 entry;
        HMODULE hMods[1024];
        DWORD cbNeeded;
        LPCVOID address = 0;

        // Create snapshots of processes
        entry.dwSize = sizeof(PROCESSENTRY32);
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

        // Get Process from processes snapshot
        if (Process32First(snapshot, &entry) == TRUE)
        {
            while (Process32Next(snapshot, &entry) == TRUE)
            {
                // Check for target process that match with moduleName
                std::string exeFile = entry.szExeFile;
                if (exeFile.find(moduleName) != std::string::npos)
                {
                    // Process found, attempt to open process
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
        {
            std::fprintf(stderr, "MAISENSE: Failed to open process\n");
            return address;
        }

        // Enumerate the list of loaded module in the process handle
        if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
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

        baseAddress = address;
        return address;
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
            std::fprintf(stderr, "MAISENSE: Failed to write process memory.\n");

        return bytesWritten;
    }
}