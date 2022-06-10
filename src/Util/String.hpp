#ifndef DRK_UTIL_STRING_HPP
#define DRK_UTIL_STRING_HPP

#include "Core/Base.hpp"

#include <string>
#include <string_view>
#include <optional>

namespace DrkCraft
{
    std::string capitalize(std::string_view str);
    std::string to_lower(std::string_view str);
    std::string to_upper(std::string_view str);

    constexpr bool is_digit(const char c)
    {
        return '0' <= c && c <= '9';
    }

    constexpr uint to_digit(const char c)
    {
        return is_digit(c) ? c - '0' : 0;
    }

    constexpr std::optional<uint> to_uint(const std::string_view str)
    {
        uint value = 0;
        uint multiplier = 1;
        for (auto c = str.rbegin(); c != str.rend(); ++c)
        {
            if (!is_digit(*c))
                return value;

            value += to_digit(*c) * multiplier;
            multiplier *= 10;
        }
        return value;
    }

    constexpr std::optional<int> to_int(const std::string_view str)
    {
        if (str.length() <= 0)
            return {};

        if (str[0] == '-')
        {
            if (const auto val = to_uint({str.data() + 1, str.length() - 1}); val.has_value())
                return 0 - *val;
            else
                return {};
        }
        else
        {
            return to_uint(str);
        }
    }
}

#endif // DRK_UTIL_STRING_HPP
