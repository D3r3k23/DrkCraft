#ifndef DRK_UTIL_RANDOM_HPP
#define DRK_UTIL_RANDOM_HPP

#include "Core/Base.hpp"

#include <random>

namespace DrkCraft
{
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
        float operator()(void);

    private:
        RandomEngine rng;
        std::uniform_real_distribution<float> dist;
    };
}

#endif // DRK_UTIL_RANDOM_HPP
