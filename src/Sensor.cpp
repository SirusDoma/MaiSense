#include <MaiSense/Sensor.hpp>

#include <ctime>

namespace MaiSense
{

    Sensor::Sensor() :
        activeFlags(0),
        inactiveFlags(0)
    {
    }

    Sensor::~Sensor()
    {
    }

    bool Sensor::Connect()
    {
        // Process hook is created, no need to recreate
        if (activeFlags && inactiveFlags)
            return true;

        // Lookup for sensor addresses
        int input = *(int*)(TOUCH_POINTER_ADDRESS);
        if (input)
        {
            // Read sensor flag from given address
            // TODO: Add P2 Support(?)
            activeFlags   = (int*)(input + P1_OFFSET_ACTIVE_ADDRESS);
            inactiveFlags = (int*)(input + P1_OFFSET_INACTIVE_ADDRESS);

            return true;
        }

        return false;
    }

    bool Sensor::SetSensorState(SensorId sensorId, bool value)
    {
        // Attempt to read sensor state in case not initialized yet
        if (!Connect())
            return false;

        // Validate Sensor Id (there's still so much gap tho)
        if (sensorId < Sensor::A1 || sensorId > Sensor::C)
            return false;

        // No need to trigger the sensor if sensor already in desired state
        if (states.count(sensorId) > 0 && states[sensorId] == value)
            return true;

        // Update sensor states
        states[sensorId] = value;

        // Write sensor bits into memory
        if (value)
        {
            *activeFlags   |= sensorId;
            *inactiveFlags &= ~sensorId;
        }
        else
        {
            *activeFlags   &= ~sensorId;
            *inactiveFlags |= sensorId;
        }

        return true;
    }

    bool Sensor::Remove(SensorId sensorId, bool value)
    {
        // Attempt to read sensor state in case not initialized yet
        if (!Connect())
            return false;

        // Validate Sensor Id (there's still so much gap tho)
        if (sensorId < Sensor::A1 || sensorId > Sensor::C)
            return false;

        // Remove the flag
        states.erase(sensorId);
        *activeFlags   &= ~sensorId;
        *inactiveFlags &= ~sensorId;
 
        return true;
    }

    void Sensor::Queue(SensorId sensorId, bool value)
    {
        auto message = Message();
        message.SensorId = sensorId;
        message.Value    = value;

        queue.push(message);
    }

    bool Sensor::Activate(SensorId sensorId)
    {
        return SetSensorState(sensorId, true);
    }

    bool Sensor::Deactivate(SensorId sensorId)
    {
        return SetSensorState(sensorId, false);
    }
    
    bool Sensor::ProcessQueue()
    {
        int processed = 0;
        while (!queue.empty())
        {
            auto message = queue.front();
            SetSensorState(message.SensorId, message.Value);

            processed++;
            queue.pop();
        }

        return processed > 0;
    }

    void Sensor::Reset()
    {
        // Attempt to read sensor state in case not initialized yet
        if (!Connect())
            return;

        // Reset sensor bitflag each game frame
        *activeFlags   = 0;
        *inactiveFlags = 0;
    }
}