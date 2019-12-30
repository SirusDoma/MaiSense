# MaiSense #

- **Author**: SirusDoma
- **Email**: com@cxo2.me
- **Latest Version**: 0.8.5

Touch Sensor API for MaiMai Finale.  
This API write the state of sensors directly into game memory.

## Usage ##

1. Build the project
2. include the `include` folder and add `Sensor.hpp` into your project.
3. Instantiate `MaiSense::Sensor` class, you can also specify module name (e.g `maimai.exe`).
4. Activate process hook by calling `MaiSense::Sensor::Connect()`.
5. Activate and Deactivate sensor by calling `MaiSense::Sensor::Activate()` and `MaiSense::Sensor::Deactivate()` respectively.

Example Code:
```
#include "Sensor.hpp"

int main()
{
    // Initialize sensor client
    // you can leave the argument empty and it will search process name that include "maimai"
    auto sensor = MaiSense::Sensor("maimai_dump_.exe"); 

    // Best do this in separate thread;
    while (true)
    {
        // Do your input checking here
        if (RawInput or Touchscreen or whatever condition......)
          sensor.Activate(MaiSensor::Sensor::A7); // turn on A7 sensor
        else
          sensor.Deactivate(MaiSensor::Sensor::A7); // turn off A7 sensor
        
        // Let's not take the entire cpu
        Sleep(100);
    }
    
    return 0;
}
```

## License ##

This is an open-sourced library licensed under the [MIT License](http://github.com/SirusDoma/MaiSense/blob/master/LICENSE)
