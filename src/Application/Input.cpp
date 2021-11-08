#include "Input.hpp"

#include "Application.hpp"

#include <GLFW/glfw3.h>

namespace DrkCraft
{
    static GLFWwindow* get_window(void)
    {
        return Application::get_instance().get_window().get_native_window();
    }

    bool is_key_pressed(KeyCode key)
    {
        auto window = get_window();
        auto status = glfwGetKey(window, key_code_to_glfw(key));
        return status == GLFW_PRESS || status == GLFW_REPEAT;
    }

    bool is_mouse_button_pressed(MouseCode button)
    {
        auto window = get_window();
        auto status = glfwGetMouseButton(window, mouse_code_to_glfw(button));
        return status == GLFW_PRESS;
    }

    glm::vec2 get_mouse_position(void)
    {
        auto window = get_window();
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

    KeyCode key_code_from_glfw(int glfwKey)
    {
        return static_cast<KeyCode>(glfwKey);
    }

    MouseCode mouse_code_from_glfw(int glfwButton)
    {
        return static_cast<MouseCode>(glfwButton);
    }

    int key_code_to_glfw(KeyCode key)
    {
        return static_cast<int>(key);
    }

    int mouse_code_to_glfw(MouseCode button)
    {
        return static_cast<int>(button);
    }

    uint16 get_code(KeyCode key)
    {
        return static_cast<uint16>(key);
    }

    uint16 get_code(MouseCode button)
    {
        return static_cast<uint16>(button);
    }
}
