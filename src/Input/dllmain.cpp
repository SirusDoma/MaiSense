#include "pch.h"

#include <MaiSense/Sensor.hpp>
#include <MaiSense/Input/InputManager.hpp>
#include <MaiSense/Input/KeyboardController.hpp>

#include <string>

using namespace MaiSense;

KeyboardController keyboardController;

BOOL APIENTRY DllMain(HMODULE hMod, DWORD cause, LPVOID lpReserved)
{
    if (!InputManager::Ready())
        return TRUE;

    if (cause == DLL_PROCESS_ATTACH) 
    {
        keyboardController.SetCallback([&](KeyEvent ev)
        {
            auto sensor = InputManager::GetSensor();
            switch (ev.KeyCode)
            {
            case 0x30: // 0
                sensor->Queue(Sensor::C, ev.Active);
                break;
            case 0x31: // 1
                sensor->Queue(Sensor::A1, ev.Active);
                break;
            case 0x32: // 2
                sensor->Queue(Sensor::A2, ev.Active);
                break;
            case 0x33: // 3
                sensor->Queue(Sensor::A3, ev.Active);
                break;
            case 0x34: // 4
                sensor->Queue(Sensor::A4, ev.Active);
                break;
            case 0x35: // 5
                sensor->Queue(Sensor::A5, ev.Active);
                break;
            case 0x36: // 6
                sensor->Queue(Sensor::A6, ev.Active);
                break;
            case 0x37: // 7
                sensor->Queue(Sensor::A7, ev.Active);
                break;
            case 0x38: // 8
                sensor->Queue(Sensor::A8, ev.Active);
                break;
            case VK_NUMPAD1: // NUMPAD 1
                sensor->Queue(Sensor::B1, ev.Active);
                break;
            case VK_NUMPAD2: // NUMPAD 2
                sensor->Queue(Sensor::B2, ev.Active);
                break;
            case VK_NUMPAD3: // NUMPAD 3
                sensor->Queue(Sensor::B3, ev.Active);
                break;
            case VK_NUMPAD4: // NUMPAD 4
                sensor->Queue(Sensor::B4, ev.Active);
                break;
            case VK_NUMPAD5: // NUMPAD 5
                sensor->Queue(Sensor::B5, ev.Active);
                break;
            case VK_NUMPAD6: // NUMPAD 6
                sensor->Queue(Sensor::B6, ev.Active);
                break;
            case VK_NUMPAD7: // NUMPAD 7
                sensor->Queue(Sensor::B7, ev.Active);
                break;
            case VK_NUMPAD8: // NUMPAD 8
                sensor->Queue(Sensor::B8, ev.Active);
                break;
            }
        });

        InputManager::Hook();
        InputManager::Install(&keyboardController);
    }
    else if (cause == DLL_PROCESS_DETACH) 
    {
        InputManager::Unhook();
    }

    return TRUE;
}
