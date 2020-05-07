#ifndef MAISENSE_TOUCH_EVENT_HPP
#define MAISENSE_TOUCH_EVENT_HPP

namespace MaiSense
{
    struct TouchEvent
    {
        unsigned int Id;
        int X;
        int Y;
        unsigned int Flag;
    };
}

#endif