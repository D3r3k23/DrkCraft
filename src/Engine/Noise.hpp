#ifndef NOISE_HPP
#define NOISE_HPP

#include "Core/Base.hpp"

#include <glm/vec2.hpp>

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
        Noise(const NoiseSpec& spec, glm::uvec2 size, int seed);
        ~Noise(void);

        Noise(Noise&& other);
        // Noise& operator=(Noise&& other);

        Noise(const Noise&) = delete;
        Noise& operator=(const Noise&) = delete;

        float get(glm::uvec2 point);
        float get(uint x, uint y);

    private:
        uint data_index(uint x, uint y);

    private:
        glm::uvec2 m_size;
        float* m_data;
    };
}

#endif // NOISE_HPP
