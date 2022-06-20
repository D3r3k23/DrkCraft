#ifndef DRK_SYSTEM_INPUT_HPP
#define DRK_SYSTEM_INPUT_HPP

#include "Core/Base.hpp"
#include "System/KeyCodes.hpp"
#include "System/MouseCodes.hpp"

#include "lib/glm/vec2.hpp"

#include <string_view>

namespace DrkCraft
{
    ////// InputCode //////

    using InputCode = Variant<MonoState, KeyCode, MouseCode>;

    InputCode to_input_code(std::string_view str);
    std::string_view input_code_name(InputCode code);

    ////// Input Queries //////

    bool is_key_pressed(KeyCode key);
    bool is_mouse_button_pressed(MouseCode button);
    bool is_pressed(InputCode code);

    vec2 get_mouse_position(void);
    float get_mouse_x(void);
    float get_mouse_y(void);

    ////// Key Mods //////

    using KeyModFlags = uint;

    enum class KeyMod : KeyModFlags
    {
        None = 0,

        Shift    = 1 << 0,
        Ctrl     = 1 << 1,
        Alt      = 1 << 2,
        Super    = 1 << 3,
        CapsLock = 1 << 4,
        NumLock  = 1 << 5
    };

    bool is_mod_pressed(KeyMod mod);

    KeyModFlags to_key_mod_flags(int mods);
    KeyModFlags to_key_mod_flags(KeyMod mod);

    bool key_mod_flags_has_mod(KeyModFlags keyModFlag, KeyModFlags flags);

    bool operator==(KeyModFlags keyModFlag, KeyMod mod);
    bool operator!=(KeyModFlags keyModFlag, KeyMod mod);

    KeyModFlags operator|(KeyModFlags flags, KeyMod mod);
    KeyModFlags operator|(KeyMod mod, KeyModFlags flags);
    KeyModFlags operator|(KeyMod mod1, KeyMod mod2);
    KeyModFlags operator|=(KeyModFlags& flags, KeyMod mod);

    KeyModFlags operator&(KeyModFlags flags, KeyMod mod);
    KeyModFlags operator&(KeyMod mod, KeyModFlags flags);
    KeyModFlags operator&(KeyMod mod1, KeyMod mod2);
    KeyModFlags operator&=(KeyModFlags& flags, KeyMod mod);
}

#endif // DRK_SYSTEM_INPUT_HPP
