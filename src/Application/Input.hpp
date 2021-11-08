#ifndef DRK_INPUT_HPP
#define DRK_INPUT_HPP

#include "Core/Base.hpp"
#include "KeyCodes.hpp"
#include "MouseCodes.hpp"

#include <glm/vec2.hpp>

namespace DrkCraft
{
    bool is_key_pressed(KeyCode key);
    bool is_mouse_button_pressed(MouseCode button);

    glm::vec2 get_mouse_position(void);
    float get_mouse_x(void);
    float get_mouse_y(void);

    KeyCode key_code_from_glfw(int glfwKey);
    MouseCode mouse_code_from_glfw(int glfwButton);

    int key_code_to_glfw(KeyCode key);
    int mouse_code_to_glfw(MouseCode button);

    uint16 get_code(KeyCode key);
    uint16 get_code(MouseCode button);
}

#endif // DRK_INPUT_HPP
