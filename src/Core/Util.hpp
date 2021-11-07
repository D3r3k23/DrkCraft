#ifndef DRK_UTIL_HPP
#define DRK_UTIL_HPP

#include <random>

namespace DrkCraft
{
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

    class RandomDoubleDist : public RandomDist
    {
    public:
        RandomDoubleDist(double min, double max);
        double get(void);
        double operator() (void);

    private:
        std::uniform_real_distribution<double> dist;
    };
}

#endif // DRK_UTIL_HPP
