#include "Debug.hpp"

#include <nameof.hpp>

namespace DrkCraft
{
    #if 0 // defined(DRK_CONFIG_DEBUG)

        std::string stringify_key_code(KeyCode key)
        {
            return NAMEOF_ENUM(key);

            int code = from_key_code(key);

            if (48 <= code && code <= 57) { // Digit
                return std::string("D") + (char)(code - 48 + '0');
            }
            else if (65 <= code && code <= 90) { // Letter
                return std::string((char)(code - 65 + 'A'));
            }
            else if (290 <= code && code <= 314) { // Fn
                return std::string("F") + (char)(code - 290 + '0');
            }
            else if (320 <= code && code <= 329) { // Keypad
                return std::string("FP_") + (char)(code - 320 + '0');
            }
            else {
                switch (code)
                {
                    case 32  : return "Space";
                    case 29  : return "Apostrophe";
                    case 44  : return "Comma";
                    case 45  : return "Minus";
                    case 46  : return "Period";
                    case 47  : return "Slash";
                    case 59  : return "Semicolon";
                    case 61  : return "Equal";
                    case 91  : return "LeftBracket";
                    case 92  : return "Backslash";
                    case 93  : return "RightBracket";
                    case 96  : return "GraveAccent";
                    case 161 : return "World1";
                    case 162 : return "World2";
                    case 256 : return "Escape";
                    case 257 : return "Enter";
                    case 258 : return "Tab";
                    case 259 : return "Backspace";
                    case 260 : return "Insert";
                    case 261 : return "Delete";
                    case 262 : return "Right";
                    case 263 : return "Left";
                    case 264 : return "Down";
                    case 265 : return "Up";
                    case 266 : return "PageUp";
                    case 267 : return "PageDown";
                    case 268 : return "Home";
                    case 269 : return "End";
                    case 280 : return "CapsLock";
                    case 281 : return "ScrollLock";
                    case 282 : return "NumLock";
                    case 283 : return "PrintScreen";
                    case 284 : return "Pause";
                    case 330 : return "KP_Decimal";
                    case 331 : return "KP_Divide";
                    case 332 : return "KP_Multiply";
                    case 333 : return "KP_Subtract";
                    case 334 : return "KP_Add";
                    case 335 : return "KP_Enter";
                    case 336 : return "KP_Equal";
                    case 340 : return "LeftShift";
                    case 341 : return "LeftCtrl";
                    case 342 : return "LeftAlt";
                    case 343 : return "LeftSuper";
                    case 344 : return "RightShift";
                    case 345 : return "RightCtr";
                    case 346 : return "RightAlt";
                    case 347 : return "RightSuper";
                    case 348 : return "Menu";
                    default  : return "None";
                }
            }
        }

        std::string stringify_mouse_code(MouseCode button)
        {
            return NAMEOF_ENUM(button);

            int code = from_mouse_code(button);

            if (3 <= code && code <= 7) {
                return std::string("Button") + (char)(code + '0');
            }
            else {
                switch (code)
                {
                    case 0  : return "Left";
                    case 1  : return "Right";
                    case 2  : return "Middle";
                    default : return "None";
                }
            }
        }

    #endif
}
