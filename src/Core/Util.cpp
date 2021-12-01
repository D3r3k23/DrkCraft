#include "Util.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <locale>

namespace DrkCraft
{
    std::string read_file(std::filesystem::path path)
    {
        if (std::filesystem::is_regular_file(path))
        {
            std::ifstream file(path);
            std::stringstream contents;
            contents << file.rdbuf();
            return contents.str();
        }
        else
            return "";
    }

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
        std::transform(str.begin(), str.end(), lower.begin(), [](char c)
        {
            return std::tolower(c, currentLocale);
        });
        return lower;
    }

    std::string to_upper(std::string_view str)
    {
        std::string upper;
        std::transform(str.begin(), str.end(), upper.begin(), [](char c)
        {
            return std::toupper(c, currentLocale);
        });
        return upper;
    }

    RandomEngine get_random_engine(void)
    {
        std::random_device rd;
        return RandomEngine(rd());
    }

    RandomIntDist::RandomIntDist(int min, int max)
      : rng(get_random_engine()),
        dist(min, max)
    { }

    int RandomIntDist::get(void)
    {
        return dist(rng);
    }

    int RandomIntDist::operator()(void)
    {
        return get();
    }

    RandomFloatDist::RandomFloatDist(float min, float max)
      : rng(get_random_engine()),
        dist(min, max)
    { }

    float RandomFloatDist::get(void)
    {
        return dist(rng);
    }

    float RandomFloatDist::operator() (void)
    {
        return get();
    }
}
