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

    using InputModFlags = uint;

    enum class KeyMod : InputModFlags
    {
        None = 0,

        Shift    = 0b000001,
        Ctrl     = 0b000010,
        Alt      = 0b000100,
        Super    = 0b001000,
        CapsLock = 0b010000,
        NumLock  = 0b100000,
    };

    InputModFlags get_input_mod_flags(int mods);
    KeyMod to_key_mod(KeyCode key);

    InputModFlags operator|(InputModFlags flags, KeyMod mod);
    InputModFlags operator|(KeyMod mod, InputModFlags flags);
    InputModFlags operator|(KeyMod mod1, KeyMod mod2);

    bool operator==(InputModFlags flags, KeyMod mod);
    bool operator==(KeyMod mod, InputModFlags flags);

    bool operator!=(InputModFlags flags, KeyMod mod);
    bool operator!=(KeyMod mod, InputModFlags flags);
}

#endif // DRK_INPUT_HPP
