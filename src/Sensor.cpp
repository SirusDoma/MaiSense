#include <MaiSense/Sensor.hpp>

#include <ctime>
#include <MaiSense/Process.hpp>

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
        // Process hook is created, no need to recreate
        if (process)
            return true;

        // Attempt to create process hook
        process = new Process(moduleName);
        if (process && process->GetProcessHandle())
        {
            // Hook created, find the pointer address of sensor
            // TODO: Add P2 Support(?)
            sensorAddress = process->Read(TOUCH_POINTER_ADDRESS) + P1_PADDING_ADDRESS;
            return true;
        }

        return false;
    }

    bool Sensor::Update(SensorId sensorId, bool value)
    {
        // Attempt to estabilish non existent hook before updating sensor
        if (!process || !Connect())
            return false;

        // Validate Sensor Id (there's still so much gap tho)
        if (sensorId < Sensor::A1 || sensorId > Sensor::C)
            return false;

        // No need to trigger the sensor if sensor already in desired state
        if (states[sensorId] == value)
            return true;

        // Update sensor states
        int sensor = 0;
        states[sensorId] = value;

        // Build sensor bits
        for (auto state : states)
        {
            if (state.second)
                sensor |= state.first;
        }

        // Write sensor bits into target process, revert state when failed
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
    
    Process *Sensor::GetProcess()
    {
        return process;
    }
}