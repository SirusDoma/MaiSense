#ifndef MAISENSE_TOUCH_CONTROLLER_HPP
#define MAISENSE_TOUCH_CONTROLLER_HPP

#include <vector>
#include <functional>
#include <Windows.h>

#include <MaiSense/InputController.hpp>
#include <MaiSense/TouchEvent.hpp>

namespace MaiSense
{
    class TouchController : public InputController
    {
    private:
        std::function<void(TouchEvent)> callback;

        virtual void OnInput(int nCode, WPARAM wParam, LPARAM lParam);

    public:
        TouchController();
        virtual ~TouchController();

        virtual bool Check(int evCode);
        void SetCallback(std::function<void(TouchEvent)> cb);
    };
}

#endif