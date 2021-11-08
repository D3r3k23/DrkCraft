#ifndef DRK_MOUSE_CODE_HPP
#define DRK_MOUSE_CODE_HPP

#include "Core/Base.hpp"

namespace DrkCraft
{
    // From glfw3.h
    enum class MouseCode : uint16
    {
        Button0 = 0,
        Button1 = 1,
        Button2 = 2,
        Button3 = 3,
        Button4 = 4,
        Button5 = 5,
        Button6 = 6,
        Button7 = 7,

        Left   = Button0,
        Right  = Button1,
        Middle = Button2
    };
}

#endif // DRK_MOUSE_CODE_HPP
