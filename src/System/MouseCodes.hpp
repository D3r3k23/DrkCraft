#ifndef DRK_SYSTEM_MOUSE_CODES_HPP
#define DRK_SYSTEM_MOUSE_CODES_HPP

#include <string_view>

namespace DrkCraft
{
    enum class MouseCode;

    MouseCode to_mouse_code(int button);
    int from_mouse_code(MouseCode button);

    MouseCode to_mouse_code(std::string_view str);
    std::string mouse_code_name(MouseCode button);

    // From glfw3.h, same codes as in imgui_impl_glfw.cpp
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
}

#endif // DRK_SYSTEM_MOUSE_CODES_HPP
