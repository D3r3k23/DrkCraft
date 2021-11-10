#ifndef DRK_DEBUG_HPP
#define DRK_DEBUG_HPP

#include "BuildSettings.hpp"
#include "Base.hpp"

namespace DrkCraft
{
    #if defined(DRK_CONFIG_DEBUG)

        #include "Application/KeyCodes.hpp"
        #include "Application/MouseCodes.hpp"

        #include <string>

        std::string stringify_key_code(KeyCode key);
        std::string stringify_mouse_code(MouseCode buttons);

        std::string stringify_vec(const glm::vec& vec);
        std::string stringify_mat(const glm::vec& mat);

    #endif
}

#endif // DRK_DEBUG_HPP
