#ifndef DRK_INPUT_HPP
#define DRK_INPUT_HPP

#include "Core/Base.hpp"
#include "Core/KeyCodes.hpp"
#include "Core/MouseCodes.hpp"

#include <glm/vec2.hpp>

namespace DrkCraft
{
    ////////// Input Queries //////////

    bool is_key_pressed(KeyCode key);
    bool is_mouse_button_pressed(MouseCode button);

    glm::vec2 get_mouse_position(void);
    float get_mouse_x(void);
    float get_mouse_y(void);

    ////////// Key Mods //////////

    using InputMod = uint;

    enum class KeyMod
    {
        None = 0,

        Shift = 1,
        Ctrl  = 2,
        Alt   = 4,
        Super = 8
    };

    bool is_mod_pressed(KeyMod mod);

    InputMod get_input_mod(int mods);
    KeyMod get_key_mod(KeyCode key);

    // Determines if keyMod is part of mods
    bool operator==(InputMod mods, KeyMod keyMod);
    bool operator==(KeyMod keyMod, InputMod mods);
    bool operator!=(InputMod mods, KeyMod keyMod);
    bool operator!=(KeyMod keyMod, InputMod mods);
}

#endif // DRK_INPUT_HPP
