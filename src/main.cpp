#include "Sensor.hpp"

int main()
{
    MaiSense::Sensor sensor;
    sensor.Connect();

    sensor.Activate(MaiSense::Sensor::A7);

    Sleep(1000);

    sensor.Deactivate(MaiSense::Sensor::A7);

    return 0;
}