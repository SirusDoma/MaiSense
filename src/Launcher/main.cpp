#include <Windows.h>
#include <string>

#include <MaiSense/Launcher/Process.hpp>
#include <MaiSense/Launcher/Injector.hpp>

int main()
{
    const std::string client = "maimai_dump_.exe";
    auto process  = MaiSense::Process::Create(client, true);
    auto injector = MaiSense::Launcher::Injector(&process);
    auto remote   = injector.Inject("MaiSense.dll");

    process.Resume();
    return 0;
}