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
        return Application::get_instance().get_window().get_raw_window();
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
            case KeyMod::Shift    : return is_key_pressed(KeyCode::LeftShift) || is_key_pressed(KeyCode::RightShift);
            case KeyMod::Ctrl     : return is_key_pressed(KeyCode::LeftCtrl)  || is_key_pressed(KeyCode::RightCtrl);
            case KeyMod::Alt      : return is_key_pressed(KeyCode::LeftAlt)   || is_key_pressed(KeyCode::RightAlt);
            case KeyMod::Super    : return is_key_pressed(KeyCode::LeftSuper) || is_key_pressed(KeyCode::RightSuper);
            case KeyMod::CapsLock : return is_key_pressed(KeyCode::CapsLock); // Or get a key and check its mods
            case KeyMod::NumLock  : return is_key_pressed(KeyCode::NumLock);
            default:
                DRK_ASSERT_DEBUG(false, "Unknown KeyMod");
                return false;
        }
    }

    InputModFlags get_input_mod_flags(int mods)
    {
        InputModFlags flags = 0;
        if (mods & GLFW_MOD_SHIFT)     flags = flags | KeyMod::Shift;
        if (mods & GLFW_MOD_CONTROL)   flags = flags | KeyMod::Ctrl;
        if (mods & GLFW_MOD_ALT)       flags = flags | KeyMod::Alt;
        if (mods & GLFW_MOD_SUPER)     flags = flags | KeyMod::Super;
        if (mods & GLFW_MOD_CAPS_LOCK) flags = flags | KeyMod::CapsLock;
        if (mods & GLFW_MOD_NUM_LOCK)  flags = flags | KeyMod::NumLock;
        return flags;
    }

    InputModFlags operator|(InputModFlags flags, KeyMod mod)
    {
        return flags | static_cast<InputModFlags>(mod);
    }

    InputModFlags operator|(KeyMod mod, InputModFlags flags)
    {
        return flags | mod;
    }

    InputModFlags operator|(KeyMod mod1, KeyMod mod2)
    {
        return static_cast<InputModFlags>(mod1) | mod2;
    }

    bool operator==(InputModFlags flags, KeyMod mod)
    {
        return flags & static_cast<InputModFlags>(mod);
    }

    bool operator==(KeyMod mod, InputModFlags flags)
    {
        return flags == mod;
    }

    bool operator!=(InputModFlags flags, KeyMod mod)
    {
        return !(flags == mod);
    }

    bool operator!=(KeyMod mod, InputModFlags flags)
    {
        return !(flags == mod);
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
