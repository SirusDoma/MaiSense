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

        HANDLE hProcess;
        LPCVOID baseAddress;

        DWORD GetTargetAddress(DWORD address, bool relative);

    public:
        Process(std::string moduleName);
        virtual ~Process();

        LPCVOID GetBaseAddress();
        HANDLE *GetProcessHandle();

        unsigned int Read(DWORD address, bool relative = true);
        unsigned int Write(DWORD address, LPCVOID buffer, SIZE_T size, bool relative = true);
    };
}

#endif
