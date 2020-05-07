#include <MaiSense/TouchController.hpp>

namespace MaiSense
{
    TouchController::TouchController() :
        callback()
    {
    }

    TouchController::~TouchController()
    {
    }

    void TouchController::SetCallback(std::function<void(TouchEvent)> cb)
    {
        callback = cb;
    }

    bool TouchController::Check(int evCode)
    {
        switch (evCode)
        {
            case WM_POINTERENTER:
            case WM_NCPOINTERDOWN:
            case WM_NCPOINTERUP:
            case WM_NCPOINTERUPDATE:
            case WM_POINTERACTIVATE:
            case WM_POINTERCAPTURECHANGED:
            case WM_POINTERDOWN:
            case WM_POINTERLEAVE:
            case WM_POINTERUP:
            case WM_POINTERUPDATE:
                return true;
            default:
                return false;
        }
    }

    void TouchController::OnInput(int nCode, WPARAM wParam, LPARAM lParam)
    {
        // Parse message
        auto msg = (LPMSG)lParam;

        // Declare pointer info
        UINT32             pointerId   = GET_POINTERID_WPARAM(msg->wParam);
        POINTER_INFO       pointerInfo = { sizeof(POINTER_INFO) };
        POINTER_INPUT_TYPE pointerType = 0;

        // Parse pointer info
        if (GetPointerType(pointerId, &pointerType) && GetPointerInfo(pointerId, &pointerInfo))
        {
            // Only retrieve touch event and map coordinate into client coordinate
            POINT point = pointerInfo.ptPixelLocation;
            if (pointerType == PT_TOUCH && ScreenToClient(msg->hwnd, &point))
            {
                // Initialize touch event
                auto ev = TouchEvent();
                ev.Id   = pointerInfo.pointerId;
                ev.X    = point.x;
                ev.Y    = point.y;
                ev.Flag = pointerInfo.pointerFlags;

                // Pass event into callback
                if (callback)
                    callback(ev);
            }
        }
    }
}
