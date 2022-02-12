#include "String.hpp"

#include <algorithm>
#include <locale>
#include <cctype>

namespace DrkCraft
{
    static std::locale currentLocale;

    std::string capitalize(std::string_view str)
    {
        std::string capitalized(str);
        if (str.size() > 0)
            capitalized[0] = std::toupper(str[0], currentLocale);
        return capitalized;
    }

    std::string to_lower(std::string_view str)
    {
        std::string lower;
        std::ranges::transform(str, lower.begin(), [](char c)
        {
            return std::tolower(c, currentLocale);
        });
        return lower;
    }

    std::string to_upper(std::string_view str)
    {
        std::string upper;
        std::ranges::transform(str, upper.begin(), [](char c)
        {
            return std::toupper(c, currentLocale);
        });
        return upper;
    }
}
