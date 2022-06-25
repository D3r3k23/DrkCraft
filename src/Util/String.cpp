#include "String.hpp"

#include <algorithm>
#include <locale>
#include <cctype>

namespace DrkCraft
{
    static const std::locale s_currentLocale;

    string capitalize(string_view str)
    {
        string capitalized{str};
        if (str.size() > 0)
            capitalized[0] = std::toupper(str[0], s_currentLocale);
        return capitalized;
    }

    string to_lower(string_view str)
    {
        string lower;
        std::ranges::transform(str, lower.begin(), [](char c)
        {
            return std::tolower(c, s_currentLocale);
        });
        return lower;
    }

    string to_upper(string_view str)
    {
        string upper;
        std::ranges::transform(str, upper.begin(), [](char c)
        {
            return std::toupper(c, s_currentLocale);
        });
        return upper;
    }
}
