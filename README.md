# MaiSense #

- **Author**: SirusDoma
- **Email**: com@cxo2.me
- **Version**: 0.8.0

Touch Sensor Input for MaiMai Finale.  

It make a use of `SetWindowsHookEx` to listen `WH_GETMESSAGE` and hook any input events.  
In order to register touch input, `CreateWindowExA` is detoured to perform `RegisterTouchWindow` after the actual `CreateWindowExA` is called.

## Usage ##

1. Build the project
2. Copy `MaiSense.Launcher.exe` and `MaiSense.dll` into game directory
3. Run `MaiSense.Launcher.exe`

### Touch Hook ###

WIP: **Not yet mapped into sensor**
- Triggering touch event will produce a dummy messagebox

### Mouse Hook ###

WIP: **Not yet mapped into sensor**
- Pressing middle button click will emulate mouse into touch input natively

### Keyboard Hook ###

WIP: Currently only support keyboard input:
- Key 1\~8 as for A1\~A8 Sensors respectively
- Numpad 1\~8 as for B1\~B8 Sensors respectively

## License ##

This is an open-sourced library licensed under the [MIT License](http://github.com/SirusDoma/MaiSense/blob/master/LICENSE)
