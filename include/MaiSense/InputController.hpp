#ifndef MAISENSE_INPUT_CONTROLLER_HPP
#define MAISENSE_INPUT_CONTROLLER_HPP

#include <Windows.h>

namespace MaiSense
{
    class InputController
    {
    public:
        InputController() {}
        virtual ~InputController() {}

        virtual bool Check(int evCode) = 0;
        virtual void OnInput(int nCode, WPARAM wParam, LPARAM lParam) = 0;
    };
}

#endif