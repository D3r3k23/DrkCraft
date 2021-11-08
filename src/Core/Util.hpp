#ifndef DRK_UTIL_HPP
#define DRK_UTIL_HPP

#include <filesystem>
#include <random>

namespace DrkCraft
{
    std::string read_file(std::filesystem::path path);

    using RandomEngine = std::mt19937;

    RandomEngine get_random_engine(void);

    class RandomIntDist
    {
    public:
        RandomIntDist(int min, int max);
        int get(void);
        int operator() (void);

    private:
        RandomEngine rng;
        std::uniform_int_distribution<int> dist;
    };

    class RandomFloatDist
    {
    public:
        RandomFloatDist(float min, float max);
        float get(void);
        float operator() (void);

    private:
        RandomEngine rng;
        std::uniform_real_distribution<float> dist;
    };
}

#endif // DRK_UTIL_HPP
