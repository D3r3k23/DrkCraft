#ifndef DRK_UTIL_NOISE_HPP
#define DRK_UTIL_NOISE_HPP

#include "Core/Base.hpp"

#include "lib/glm/vec2.hpp"

namespace DrkCraft
{
    struct NoiseSpec
    {
        enum class Type
        {
            OpenSimplex2,
            OpenSimplex2S,
            Cellular,
            Perlin,
            ValueCubic,
            Value
        };

        NoiseSpec::Type type;
    };

    class Noise
    {
    public:
        Noise(void) = default;
        Noise(const NoiseSpec& spec, const uvec2& size, int seed);
        ~Noise(void);

        Noise(Noise&& other);
        Noise& operator=(Noise&& other);

        Noise(const Noise&) = delete;
        Noise& operator=(const Noise&) = delete;

        float get(const uvec2& point);
        float get(uint x, uint y);

    private:
        uint data_index(uint x, uint y);

    private:
        uvec2 m_size;
        float* m_data;
    };
}

#endif // DRK_UTIL_NOISE_HPP
