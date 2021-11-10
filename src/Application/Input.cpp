#include "Input.hpp"
#include "Core/KeyCodes.hpp"
#include "Core/MouseCodes.hpp"

#include "Application.hpp"

#include <GLFW/glfw3.h>

namespace DrkCraft
{
    ////////// Input Queries //////////

    static GLFWwindow* local_get_window(void)
    {
        return Application::get_instance().get_window().get_native_window();
    }

    bool is_key_pressed(KeyCode key)
    {
        auto window = local_get_window();
        auto status = glfwGetKey(window, from_key_code(key));
        return status == GLFW_PRESS || status == GLFW_REPEAT;
    }

    bool is_mouse_button_pressed(MouseCode button)
    {
        auto window = local_get_window();
        auto status = glfwGetMouseButton(window, from_mouse_code(button));
        return status == GLFW_PRESS;
    }

    glm::vec2 get_mouse_position(void)
    {
        auto window = local_get_window();
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        return { (float)xPos, (float)yPos };
    }

    float get_mouse_x(void)
    {
        return get_mouse_position().x;
    }

    float get_mouse_y(void)
    {
        return get_mouse_position().y;
    }

    ////////// Input Mods //////////

    bool is_mod_pressed(KeyMod mod)
    {
        switch (mod)
        {
            case KeyMod::Shift:
                return is_key_pressed(KeyCode::LeftShift) || is_key_pressed(KeyCode::RightShift);
            case KeyMod::Ctrl:
                return is_key_pressed(KeyCode::LeftCtrl) || is_key_pressed(KeyCode::RightCtrl);
            case KeyMod::Alt:
                return is_key_pressed(KeyCode::LeftAlt) || is_key_pressed(KeyCode::RightAlt);
            case KeyMod::Super:
                return is_key_pressed(KeyCode::LeftSuper) || is_key_pressed(KeyCode::RightSuper);
            default: return false;
        }
    }

    InputMod get_input_mod(int mods)
    {
        return static_cast<InputMod>(mods);
    }

    KeyMod get_key_mod(KeyCode key)
    {
        switch (key)
        {
            case KeyCode::LeftShift:
            case KeyCode::RightShift:
                return KeyMod::Shift;

            case KeyCode::LeftCtrl:
            case KeyCode::RightCtrl:
                return KeyMod::Ctrl;

            case KeyCode::LeftAlt:
            case KeyCode::RightAlt:
                return KeyMod::Alt;

            case KeyCode::LeftSuper:
            case KeyCode::RightSuper:
                return KeyMod::Super;

            default:
                return KeyMod::None;
        }
    }

    bool operator==(InputMod mods, KeyMod keyMod)
    {
        return static_cast<int>(mods) | static_cast<int>(keyMod);
    }

    bool operator==(KeyMod keyMod, InputMod mods)
    {
        return static_cast<int>(mods) | static_cast<int>(keyMod);
    }

    bool operator!=(InputMod mods, KeyMod keyMod)
    {
        return !(mods == keyMod);
    }

    bool operator!=(KeyMod keyMod, InputMod mods)
    {
        return !(mods == keyMod);
    }

    ////////// KeyCodes.hpp //////////

    KeyCode to_key_code(int key)
    {
        return static_cast<KeyCode>(key);
    }

    int from_key_code(KeyCode key)
    {
        return static_cast<int>(key);
    }

    ////////// MouseCodes.hpp //////////

    MouseCode to_mouse_code(int button)
    {
        return static_cast<MouseCode>(button);
    }

    int from_mouse_code(MouseCode button)
    {
        return static_cast<int>(button);
    }
}
