#include <MaiSense/KeyboardController.hpp>

namespace MaiSense
{
    KeyboardController::KeyboardController() :
        callback()
    {
    }

    KeyboardController::~KeyboardController()
    {
    }

    void KeyboardController::SetCallback(std::function<void(KeyEvent)> cb)
    {
        callback = cb;
    }

    bool KeyboardController::Check(int evCode)
    {
        switch (evCode)
        {
        case WM_KEYDOWN:
        case WM_KEYUP:
            return true;
        default:
            return false;
        }
    }

    void KeyboardController::OnInput(int nCode, WPARAM wParam, LPARAM lParam)
    {
        // Parse keyboard hook event
        auto ev  = KeyEvent();
        auto msg = (LPMSG)lParam;

        // Initialize key event
        ev.KeyCode = msg->wParam;
        ev.Active  = msg->message == WM_KEYDOWN;

        // Pass event into callback
        if (callback)
            callback(ev);
    }
}
