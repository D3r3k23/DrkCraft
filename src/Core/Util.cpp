#include "Util.hpp"

namespace DrkCraft
{
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

    RandomDoubleDist::RandomDoubleDist(double min, double max)
      : RandomDist(),
        dist(min, max)
    { }

    double RandomDoubleDist::get(void)
    {
        return dist(rng);
    }

    double RandomDoubleDist::operator() (void)
    {
        return get();
    }
}
