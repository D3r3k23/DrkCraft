#ifndef DRK_NOISE_HPP
#define DRK_NOISE_HPP

#include "Core/Base.hpp"

#include <FastNoiseLite.h>
#include <glm/vec2.hpp>

namespace DrkCraft
{
    class Noise
    {
    public:
        Noise(FastNoiseLite& generator, glm::uvec2 size);
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

#endif // DRK_NOISE_HPP
