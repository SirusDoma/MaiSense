#ifndef MAISENSE_MAIN_HPP
#define MAISENSE_MAIN_HPP

#include <string>
#include <windows.h>
#include <unordered_map> 

typedef int SensorId;

namespace MaiSense
{
    class Process;
    class Sensor
    {
    private:
        const DWORD TOUCH_POINTER_ADDRESS = 0xB40D28;
        const DWORD P1_PADDING_ADDRESS = 52;
        const DWORD P2_PADDING_ADDRESS = 56;

        Process *process;
        DWORD sensorAddress;

        std::string moduleName;
        std::unordered_map<SensorId, bool> states;

    public:
        static const SensorId A1 = 1 << 0,
            B1 = 1 << 1,
            A2 = 1 << 2,
            B2 = 1 << 3,
            A3 = 1 << 5,
            B3 = 1 << 6,
            A4 = 1 << 7,
            B4 = 1 << 8,
            A5 = 1 << 10,
            B5 = 1 << 11,
            A6 = 1 << 12,
            B6 = 1 << 13,
            A7 = 1 << 15,
            B7 = 1 << 16,
            A8 = 1 << 17,
            B8 = 1 << 18,
            C  = 1 << 19;

        Sensor();
        Sensor(const std::string &moduleName);
        ~Sensor();

        bool Connect();

        bool Update(SensorId sensorId, bool value);
        bool Activate(SensorId sensorId);
        bool Deactivate(SensorId sensorId);

        Process *GetProcess();
    };
}

#endif