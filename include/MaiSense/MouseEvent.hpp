#ifndef MAISENSE_MOUSE_EVENT_HPP
#define MAISENSE_MOUSE_EVENT_HPP

namespace MaiSense
{
    struct MouseEvent
    {
        int X;
        int Y;
        bool LButton;
        bool MButton;
        bool RButton;
    };
}

#endif