#ifndef DRK_MOUSE_CODES_HPP
#define DRK_MOUSE_CODES_HPP

namespace DrkCraft
{
    // From glfw3.h
    enum class MouseCode
    {
        None = -1,

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

    MouseCode to_mouse_code(int button);
    int from_mouse_code(MouseCode button);
}

#endif // DRK_MOUSE_CODES_HPP
