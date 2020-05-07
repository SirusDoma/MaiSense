#include <MaiSense/MouseController.hpp>
#include <string>
#include <iostream>

namespace MaiSense
{
    MouseController::MouseController() :
        callback(),
        contact(),
        touchEmulationActive(false),
        prevEvent()
    {
    }

    MouseController::~MouseController()
    {
    }

    void MouseController::EmulateTouch()
    {
        if (touchEmulationActive)
            return;

        touchEmulationActive = true;
        
        // Setup touch contact (only use single contact, there's only single mouse after all)
        contact = { sizeof(POINTER_TOUCH_INFO) };
        contact.pointerInfo.pointerType = PT_TOUCH;
        contact.pointerInfo.pointerId = 0;

        // Set touch contact flags
        contact.touchFlags = TOUCH_FLAG_NONE;
        contact.touchMask = TOUCH_MASK_CONTACTAREA | TOUCH_MASK_ORIENTATION | TOUCH_MASK_PRESSURE;
        contact.orientation = 90; // Orientation of 90 means touching perpendicular to screen.
        contact.pressure = 32000;

        // Initialize touch injection
        if (!InitializeTouchInjection(MAX_CONTACT_SIMULATION, TOUCH_FEEDBACK_DEFAULT))
            MessageBoxA(NULL, ("MAISENSE: Failed to register touch injection: " + std::to_string(GetLastError())).c_str(), "MaiSense", MB_ICONEXCLAMATION);
    }

    void MouseController::SetCallback(std::function<void(MouseEvent)> cb)
    {
        callback = cb;
    }

    bool MouseController::Check(int evCode)
    {
        switch (evCode)
        {
        case WM_MOUSEACTIVATE:
        case WM_MOUSEMOVE:
        case WM_MOUSEWHEEL:
        case WM_MOUSEHOVER:
        case WM_MOUSELEAVE:
        case WM_NCMOUSEHOVER:
        case WM_NCMOUSEMOVE:
        case WM_NCMOUSELEAVE:
        case WM_MOUSELAST:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_NCLBUTTONDOWN:
        case WM_NCLBUTTONUP:
        case WM_NCLBUTTONDBLCLK:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
        case WM_NCRBUTTONDOWN:
        case WM_NCRBUTTONUP:
        case WM_NCRBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MBUTTONDBLCLK:
        case WM_NCMBUTTONDOWN:
        case WM_NCMBUTTONUP:
        case WM_NCMBUTTONDBLCLK:
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
        case WM_XBUTTONDBLCLK:
        case WM_NCXBUTTONDOWN:
        case WM_NCXBUTTONUP:
        case WM_NCXBUTTONDBLCLK:
            return true;
        default:
            return false;
        }
    }

    void MouseController::OnInput(int nCode, WPARAM wParam, LPARAM lParam)
    {
        // Parse message
        auto msg = (LPMSG)lParam;

        // Map coordinate into client coordinate
        POINT point = msg->pt;
        if (ScreenToClient(msg->hwnd, &point))
        {
            // Initialize touch event
            auto ev = MouseEvent();
            ev.X = point.x;
            ev.Y = point.y;

            if (msg->message != WM_MOUSEMOVE && msg->message != WM_NCMOUSEMOVE)
            {
                ev.LButton = msg->message == WM_LBUTTONDOWN;
                ev.MButton = msg->message == WM_MBUTTONDOWN;
                ev.RButton = msg->message == WM_RBUTTONDOWN;
            }
            else
            {
                ev.LButton = prevEvent.LButton;
                ev.MButton = prevEvent.MButton;
                ev.RButton = prevEvent.RButton;
            }


            // Check whether touch simulation is active
            if (touchEmulationActive)
            {
                // Set touch contact coordinate and area
                contact.pointerInfo.ptPixelLocation = msg->pt;
                contact.rcContact.top    = contact.pointerInfo.ptPixelLocation.y - 2;
                contact.rcContact.bottom = contact.pointerInfo.ptPixelLocation.y + 2;
                contact.rcContact.left   = contact.pointerInfo.ptPixelLocation.x - 2;
                contact.rcContact.right  = contact.pointerInfo.ptPixelLocation.x + 2;

                // Set touch contact state
                if (msg->message == WM_LBUTTONDOWN)
                    contact.pointerInfo.pointerFlags = POINTER_FLAG_DOWN | POINTER_FLAG_INRANGE | POINTER_FLAG_INCONTACT;
                else if (msg->message == WM_LBUTTONUP)
                    contact.pointerInfo.pointerFlags = POINTER_FLAG_UP | POINTER_FLAG_CANCELED;
                else // Mouse move or other possible events
                    contact.pointerInfo.pointerFlags = POINTER_FLAG_UPDATE;

                // Inject touch contact into screen
                contact.pointerInfo.pointerId = 0;
                InjectTouchInput(1, &contact);
            }

            // Pass event into callback
            prevEvent = ev;
            if (callback)
                callback(ev);
        }
    }
}
