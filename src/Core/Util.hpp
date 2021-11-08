#ifndef DRK_UTIL_HPP
#define DRK_UTIL_HPP

#include <filesystem>
#include <random>

namespace DrkCraft
{
    std::string read_file(std::filesystem::path path);

    class RandomDist
    {
    protected:
        RandomDist(void);

        std::random_device dev;
        std::mt19937 rng;
    };

    class RandomIntDist : public RandomDist
    {
    public:
        RandomIntDist(int min, int max);
        int get(void);
        int operator() (void);

    private:
        std::uniform_int_distribution<int> dist;
    };

    class RandomFloatDist : public RandomDist
    {
    public:
        RandomFloatDist(float min, float max);
        float get(void);
        float operator() (void);

    private:
        std::uniform_real_distribution<float> dist;
    };
}

#endif // DRK_UTIL_HPP
