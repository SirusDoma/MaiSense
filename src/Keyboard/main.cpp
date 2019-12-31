#include <MaiSense/Sensor.hpp>
#include <MaiSense/Process.hpp>
#include "Keyboard.hpp"

#include <Windows.h>

int main()
{
    auto sensor = MaiSense::Sensor();
    if (!sensor.Connect())
    {
        std::fprintf(stderr, "MAISENSE: Failed to connect into process.\n");
        return -1;
    }

    if (!MaiSense::Input::Keyboard::Initialize())
    {
        std::fprintf(stderr, "MAISENSE: Failed to hook keyboard input.\n");
        return -1;
    }

    MaiSense::Input::Keyboard::SetCallback([&](MaiSense::Input::KeyEvent ev) {
        // For keycode refer to following doc:
        // https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
        switch (ev.KeyCode)
        {
        case 0x30: // 0
            sensor.Update(MaiSense::Sensor::C, ev.Active);
            break;
        case 0x31: // 1
            sensor.Update(MaiSense::Sensor::A1, ev.Active);
            break;
        case 0x32: // 2
            sensor.Update(MaiSense::Sensor::A2, ev.Active);
            break;
        case 0x33: // 3
            sensor.Update(MaiSense::Sensor::A3, ev.Active);
            break;
        case 0x34: // 4
            sensor.Update(MaiSense::Sensor::A4, ev.Active);
            break;
        case 0x35: // 5
            sensor.Update(MaiSense::Sensor::A5, ev.Active);
            break;
        case 0x36: // 6
            sensor.Update(MaiSense::Sensor::A6, ev.Active);
            break;
        case 0x37: // 7
            sensor.Update(MaiSense::Sensor::A7, ev.Active);
            break;
        case 0x38: // 8
            sensor.Update(MaiSense::Sensor::A8, ev.Active);
            break;
        case VK_NUMPAD1: // NUMPAD 1
            sensor.Update(MaiSense::Sensor::B1, ev.Active);
            break;
        case VK_NUMPAD2: // NUMPAD 2
            sensor.Update(MaiSense::Sensor::B2, ev.Active);
            break;
        case VK_NUMPAD3: // NUMPAD 3
            sensor.Update(MaiSense::Sensor::B3, ev.Active);
            break;
        case VK_NUMPAD4: // NUMPAD 4
            sensor.Update(MaiSense::Sensor::B4, ev.Active);
            break;
        case VK_NUMPAD5: // NUMPAD 5
            sensor.Update(MaiSense::Sensor::B5, ev.Active);
            break;
        case VK_NUMPAD6: // NUMPAD 6
            sensor.Update(MaiSense::Sensor::B6, ev.Active);
            break;
        case VK_NUMPAD7: // NUMPAD 
            sensor.Update(MaiSense::Sensor::B7, ev.Active);
            break;
        case VK_NUMPAD8: // NUMPAD 8
            sensor.Update(MaiSense::Sensor::B8, ev.Active);
            break;
        }
    });
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        auto ev = MaiSense::Input::Keyboard::GetLastKeyEvent();
        if (!msg.hwnd || ev.KeyCode == VK_ESCAPE)
            return 0;
    }

    return 0;
}