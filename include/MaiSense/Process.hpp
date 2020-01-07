#ifndef MAISENSE_PROCESS_HPP
#define MAISENSE_PROCESS_HPP

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <string>

namespace MaiSense
{
    class Process
    {
    private:
        std::string moduleName;

        int processId, threadId;
        HANDLE hProcess, hThread;
        LPCVOID hModule;

        DWORD GetTargetAddress(DWORD address, bool relative);

    public:
        Process(std::string moduleName);
        Process(HANDLE hProcess, HANDLE hThread, int processId, int threadId);

        virtual ~Process();

        static Process Create(std::string path, bool suspended = false);
        static Process Create(std::string path, std::string args = "", bool suspended = false);

        int GetProcessId();
        int GetThreadId();

        HANDLE  GetProcessHandle();
        HANDLE  GetThreadHandle();
        LPCVOID GetBaseAddress();

        bool Resume();
        bool Suspend();

        void Wait();

        unsigned int Read(DWORD address, bool relative = true);
        unsigned int Write(DWORD address, LPCVOID buffer, SIZE_T size, bool relative = true);
    };
}

#endif
