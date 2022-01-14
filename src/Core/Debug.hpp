#ifndef DRK_DEBUG_HPP
#define DRK_DEBUG_HPP

#include "Base.hpp"
#include "BuildSettings.hpp"

namespace DrkCraft
{
    #if 0 // defined(DRK_CONFIG_DEBUG)

        #include "System/KeyCodes.hpp"
        #include "System/MouseCodes.hpp"

        #include <string>

        std::string stringify_key_code(KeyCode key);
        std::string stringify_mouse_code(MouseCode buttons);

        std::string stringify_vec(const glm::vec& vec);
        std::string stringify_mat(const glm::vec& mat);

    #endif
}

#endif // DRK_DEBUG_HPP
