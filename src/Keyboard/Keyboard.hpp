#ifndef MAISENSE_KEYBOARD_HPP
#define MAISENSE_KEYBOARD_HPP

#include <vector>
#include <functional>
#include <Windows.h>

#include <MaiSense/Sensor.hpp>
#include "KeyEvent.hpp"

namespace MaiSense::Input
{
    class Keyboard
    {
    private:
        static HHOOK hook;
        static KeyEvent lastEvent;
        static std::function<void(KeyEvent)> callback;

        static LRESULT __stdcall OnKeyPress(int nCode, WPARAM wParam, LPARAM lParam);
    public:
        static bool Initialize();
        static bool Dispose();

        static KeyEvent GetLastKeyEvent();
        static void SetCallback(std::function<void(KeyEvent)> cb);
    };
}

#endif