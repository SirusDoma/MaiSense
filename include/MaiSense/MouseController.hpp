#ifndef MAISENSE_MOUSE_CONTROLLER_HPP
#define MAISENSE_MOUSE_CONTROLLER_HPP

#include <vector>
#include <functional>
#include <Windows.h>

#include <MaiSense/InputController.hpp>
#include <MaiSense/MouseEvent.hpp>

namespace MaiSense
{
    class MouseController : public InputController
    {
    private:
        const int MAX_CONTACT_SIMULATION = 32;

        POINTER_TOUCH_INFO contact;
        bool touchEmulationActive;

        MouseEvent prevEvent;
        std::function<void(MouseEvent)> callback;

        virtual void OnInput(int nCode, WPARAM wParam, LPARAM lParam);

    public:
        MouseController();
        virtual ~MouseController();

        void EmulateTouch();
        virtual bool Check(int evCode);
        void SetCallback(std::function<void(MouseEvent)> cb);
    };
}

#endif