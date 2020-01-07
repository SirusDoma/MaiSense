#ifndef MAISENSE_KEYBOARD_CONTROLLER_HPP
#define MAISENSE_KEYBOARD_CONTROLLER_HPP

#include <vector>
#include <functional>
#include <Windows.h>

#include <MaiSense/Input/InputController.hpp>
#include <MaiSense/Input/KeyEvent.hpp>

namespace MaiSense
{
    class KeyboardController : public InputController
    {
    private:
        std::function<void(KeyEvent)> callback;
        virtual void OnInput(int nCode, WPARAM wParam, LPARAM lParam);

    public:
        KeyboardController();
        virtual ~KeyboardController();

        virtual bool Check(int evCode);
        void SetCallback(std::function<void(KeyEvent)> cb);
    };
}

#endif