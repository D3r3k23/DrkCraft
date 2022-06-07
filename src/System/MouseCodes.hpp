#ifndef DRK_SYSTEM_MOUSE_CODES_HPP
#define DRK_SYSTEM_MOUSE_CODES_HPP

#include <string_view>

namespace DrkCraft
{
    enum class MouseCode;

    MouseCode to_mouse_code(int button);
    int from_mouse_code(MouseCode button);

    MouseCode to_mouse_code(std::string_view str);
    std::string_view mouse_code_name(MouseCode button);

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
        Button7 = 7
    };

    namespace MouseButton
    {
        enum
        {
            Left = MouseCode::Button0,
            Right = MouseCode::Button1,
            Middle = MouseCode::Button2
        };
    }
}

#endif // DRK_SYSTEM_MOUSE_CODES_HPP
