#ifndef DRK_UTIL_STRING_HPP
#define DRK_UTIL_STRING_HPP

#include "Core/Base.hpp"

#include <string>
#include <string_view>

namespace DrkCraft
{
    std::string capitalize(std::string_view str);
    std::string to_lower(std::string_view str);
    std::string to_upper(std::string_view str);
}

#endif // DRK_UTIL_STRING_HPP
