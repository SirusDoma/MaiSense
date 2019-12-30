#include "Sensor.hpp"

#include <ctime>
#include "Process.hpp"

namespace MaiSense
{
    Sensor::Sensor()
        : Sensor::Sensor("maimai")
    {
    }

    Sensor::Sensor(const std::string &moduleName) : 
        moduleName(moduleName),
        process(0),
        sensorAddress(0)
    {
    }

    Sensor::~Sensor()
    {
        if (process)
            delete process;
    }

    bool Sensor::Connect()
    {
        if (process)
            return true;

        process = new Process(moduleName);
        if (process && process->GetProcessHandle())
        {
            sensorAddress = process->Read(TOUCH_POINTER_ADDRESS) + P1_ADDRESS;
            return true;
        }

        return false;
    }

    bool Sensor::Update(SensorId sensorId, bool value)
    {
        if (!process || !Connect())
            return false;

        if (sensorId < Sensor::A1 || sensorId > Sensor::C)
            return false;

        if (states[sensorId] == value)
            return true;

        int sensor = 0;
        states[sensorId] = value;

        for (auto state : states)
        {
            if (state.second)
                sensor |= state.first;
        }

        bool success = process->Write(sensorAddress, &sensor, sizeof(sensor), false);
        if (!success)
            states[sensorId] = !value;

        return success;
    }

    bool Sensor::Activate(SensorId sensorId)
    {
        return Update(sensorId, true);
    }

    bool Sensor::Deactivate(SensorId sensorId)
    {
        return Update(sensorId, false);
    }
}