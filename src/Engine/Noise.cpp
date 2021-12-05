#include "Noise.hpp"

namespace DrkCraft
{
    Noise::Noise(FastNoiseLite& generator, glm::uvec2 size)
      : m_size(size)
    {
        m_data = new float[size.x * size.y];

        for (uint x = 0; x < size.x; x++)
            for (uint y = 0; y < size.y; y++)
                m_data[data_index(x, y)] = generator.GetNoise((float)x, (float)y);
    }

    Noise::~Noise(void)
    {
        delete[] m_data;
    }

    Noise::Noise(Noise&& other)
    {
        m_size = other.m_size;
        m_data = other.m_data;

        other.m_size = {0, 0};
        other.m_data = nullptr;
    }

    // Noise& Noise::operator=(Noise&& other)
    // {
    //     m_size = other.m_size;
    //     m_data = other.m_data;

    //     other.m_size = {0, 0};
    //     other.m_data = nullptr;

    //     return *this;
    // }

    float  Noise::get(glm::uvec2 point)
    {
        return get(point.x, point.y);
    }

    float Noise::get(uint x, uint y)
    {
        return m_data[data_index(x, y)];
    }

    uint Noise::data_index(uint x, uint y)
    {
        DRK_ASSERT_DEBUG(x < m_size.x && y < m_size.y, "Index out of range");
        return x + m_size.x * y;
    }
}
