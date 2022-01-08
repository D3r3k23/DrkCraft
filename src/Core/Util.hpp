#ifndef DRK_UTIL_HPP
#define DRK_UTIL_HPP

#include "Base.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <string>
#include <string_view>
#include <filesystem>
#include <random>
#include <tuple>

namespace DrkCraft
{
    std::string read_file(std::filesystem::path path);

    std::string capitalize(std::string_view str);

    std::string to_lower(std::string_view str);
    std::string to_upper(std::string_view str);

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

    template <typename V>
    concept Vector2Concept = requires(V vec)
    {
        { vec.x };
        { vec.y };
    };

    template <typename V>
    concept Vector3Concept = requires(V vec)
    {
        { vec.x };
        { vec.y };
        { vec.z };
    };

    template <Vector2Concept V>
    std::tuple<decltype(V::x), decltype(V::y)> glm_bind(const V& vec)
    {
        return { vec.x, vec.y };
    }

    template <Vector3Concept V>
    std::tuple<decltype(V::x), decltype(V::y), decltype(V::z)> glm_bind(constV& vec)
    {
        return { vec.x, vec.y, vec.z };
    }
}

#endif // DRK_UTIL_HPP
