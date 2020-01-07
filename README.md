# MaiSense #

- **Author**: SirusDoma
- **Email**: com@cxo2.me
- **Version**: 0.8.5

Touch Sensor Input for MaiMai Finale.  
API need to be injected into the main client to make it work.

## Usage ##

1. Build the project
2. Copy `MaiSense.Launcher.exe` and `MaiSense.dll` into game directory
3. Run `MaiSense.Launcher.exe`

WIP: Currently only support keyboard input:
- Key 1\~8 as for A1\~A8 Sensors respectively
- Numpad 1\~8 as for B1\~B8 Sensors respectively

For the API, refer to [`Sensor.hpp`](https://github.com/SirusDoma/MaiSense.API/blob/master/include/MaiSense/Sensor.hpp).  
Make sure to include necessary headers and lib files.

## License ##

This is an open-sourced library licensed under the [MIT License](http://github.com/SirusDoma/MaiSense.API/blob/master/LICENSE)
