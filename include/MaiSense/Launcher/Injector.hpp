#ifndef MAISENSE_INJECTOR_HPP
#define MAISENSE_INJECTOR_HPP

#include <string>
#include <MaiSense/Process.hpp>

namespace MaiSense::Launcher
{
    class Injector
    {
    private:
        Process *process;
        void *remoteAddr;
        HANDLE remoteThread;

        bool Close();

    public:
        Injector(Process *process);
        HANDLE Inject(std::string dll_name);
    };
}

#endif