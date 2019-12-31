#include "Keyboard.hpp"

namespace MaiSense::Input
{
    HHOOK Keyboard::hook;
    KeyEvent Keyboard::lastEvent;
    std::function<void(KeyEvent)> Keyboard::callback;

    bool Keyboard::Initialize()
    {
        hook = SetWindowsHookEx(WH_KEYBOARD_LL, Keyboard::OnKeyPress, 0, 0);
        return hook != 0;
    }

    bool Keyboard::Dispose()
    {
        if (hook)
            return UnhookWindowsHookEx(hook);
        return false;
    }

    KeyEvent Keyboard::GetLastKeyEvent()
    {
        return lastEvent;
    }

    void Keyboard::SetCallback(std::function<void(KeyEvent)> cb)
    {
        callback = cb;
    }

    LRESULT __stdcall Keyboard::OnKeyPress(int nCode, WPARAM wParam, LPARAM lParam)
    {
        // Validate hook event
        if (nCode >= 0)
        {
            // Keypress / Keyrelease event
            if (wParam == WM_KEYDOWN || wParam == WM_KEYUP)
            {
                // lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
                auto ev = KeyEvent();
                auto input = *((KBDLLHOOKSTRUCT*)lParam);
               
                // Construct key event
                ev.KeyCode = input.vkCode;
                ev.Active  = wParam == WM_KEYDOWN;

                // Assign latest key event and trigger the callback
                lastEvent = ev;
                if (callback)
                    callback(ev);
            }
        }

        // call the next hook in the hook chain. This is nessecary or your hook chain will break and the hook stops
        return CallNextHookEx(hook, nCode, wParam, lParam);
    }
}
