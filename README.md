# MaiSense #

- **Author**: SirusDoma (a.k.a CXO2)
- **Email**: com@cxo2.me
- **Version**: 1.0.0

Touch Sensor Input for SDEY, touchlaundry disco 💦. 

## How it works ##

It make a use of `SetWindowsHookEx` to listen `WH_GETMESSAGE` and hook any input events.  
In order to register touch input, `CreateWindowExA` is detoured to perform `RegisterTouchWindow` after the actual `CreateWindowExA` is called.  

The game did **not** emulate any hardware input such as JVS and touch sensors instead, it write input flag directly to the game memory.

## Usage ##

1. [Download](https://github.com/SirusDoma/MaiSense/releases/latest) or Build the project
2. Copy `MaiSense.Launcher.exe` and `MaiSense.dll` into game directory
3. Run `MaiSense.Launcher.exe`

### Touch Hook ###

Multi touch input is supported. Make sure that touch input emulation from Mouse middle button is not activated, otherwise restart the game.  

Only support 1P with DEV mode only, has no plan to support 2P or any other mode.

### Mouse Hook ###

- Pressing middle button click will emulate mouse into touch input natively.
- Produced touch tap by the emulation only last for few millisecond or even nanosecond, so it doesn't not registered as proper touch input at most of the time.
- Native mouse event is not supported.

### Keyboard Hook ###

- Key 1\~8 as for A1\~A8 Sensors respectively, 0 for C Sensor
- Numpad 1\~8 as for B1\~B8 Sensors respectively

## License ##

This is an open-sourced library licensed under the [MIT License](http://github.com/SirusDoma/MaiSense/blob/master/LICENSE)
