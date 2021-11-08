#include "Util.hpp"

#include <fstream>
#include <sstream>

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

    RandomDist::RandomDist(void)
      : rng(dev())
    { }

    RandomIntDist::RandomIntDist(int min, int max)
      : RandomDist(),
        dist(min, max)
    { }

    int RandomIntDist::get(void)
    {
        return dist(rng);
    }

    int RandomIntDist::operator() (void)
    {
        return get();
    }

    RandomFloatDist::RandomFloatDist(float min, float max)
      : RandomDist(),
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
