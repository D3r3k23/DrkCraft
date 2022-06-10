#ifndef DRK_GAME_UTIL_NOISE_HPP
#define DRK_GAME_UTIL_NOISE_HPP

#include "Core/Base.hpp"

#include "lib/glm/vec2.hpp"

namespace DrkCraft::Game
{
    class Noise
    {
    public:
        enum class Type
        {
            OpenSimplex2,
            OpenSimplex2S,
            Cellular,
            Perlin,
            ValueCubic,
            Value
        };

        struct Spec
        {
            Type type;
        };

    public:
        Noise(void) = default;
        Noise(const Spec& spec, const uvec2& size, int seed);
        ~Noise(void);

        Noise(Noise&& other);
        Noise& operator=(Noise&& other);

        Noise(const Noise&) = delete;
        Noise& operator=(const Noise&) = delete;

        float get(const uvec2& point);
        float get(uint x, uint y);

        const Spec& info(void) const;

    private:
        uint data_index(uint x, uint y);

    private:
        Spec m_spec;
        uvec2 m_size;
        float* m_data;
    };
}

#endif // DRK_GAME_UTIL_NOISE_HPP
