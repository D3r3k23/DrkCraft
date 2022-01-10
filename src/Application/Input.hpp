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

    using KeyModFlags = uint;

    enum class KeyMod : KeyModFlags
    {
        None = 0,

        Shift    = 0b000001,
        Ctrl     = 0b000010,
        Alt      = 0b000100,
        Super    = 0b001000,
        CapsLock = 0b010000,
        NumLock  = 0b100000,
    };

    bool is_mod_pressed(KeyMod mod);

    KeyModFlags to_key_mod_flags(int mods);
    KeyModFlags to_key_mod_flags(KeyMod mod);

    bool key_mod_flag_contains(KeyModFlags flags, KeyMod mod);
    bool key_mod_flag_equals(KeyModFlags flags, KeyMod mod);

    bool operator==(KeyModFlags flags, KeyMod mod);
    bool operator!=(KeyModFlags flags, KeyMod mod);

    bool operator==(KeyMod mod, KeyModFlags flags);
    bool operator!=(KeyMod mod, KeyModFlags flags);

    KeyModFlags operator|(KeyModFlags flags, KeyMod mod);
    KeyModFlags operator|(KeyMod mod, KeyModFlags flags);
    KeyModFlags operator|(KeyMod mod1, KeyMod mod2);
    KeyModFlags operator|=(KeyModFlags& flags, KeyMod mod);

    KeyModFlags operator&(KeyModFlags flags, KeyMod mod);
    KeyModFlags operator&(KeyMod mod, KeyModFlags flags);
    KeyModFlags operator&(KeyMod mod1, KeyMod mod2);
    KeyModFlags operator&=(KeyModFlags& flags, KeyMod mod);
}

#endif // DRK_INPUT_HPP
