#include "Noise.hpp"

#include <FastNoiseLite.h>

#include <unordered_map>

namespace DrkCraft::Game
{
    namespace
    {
        void apply_fast_noise_lite_spec(FastNoiseLite& generator, const Noise::Spec& spec)
        {
            static const std::unordered_map<Noise::Type, FastNoiseLite::NoiseType> noiseTypeMap
            {
                { Noise::Type::OpenSimplex2,  FastNoiseLite::NoiseType_OpenSimplex2 },
                { Noise::Type::OpenSimplex2S, FastNoiseLite::NoiseType_OpenSimplex2S },
                { Noise::Type::Cellular,      FastNoiseLite::NoiseType_Cellular },
                { Noise::Type::Perlin,        FastNoiseLite::NoiseType_Perlin },
                { Noise::Type::ValueCubic,    FastNoiseLite::NoiseType_ValueCubic },
                { Noise::Type::Value,         FastNoiseLite::NoiseType_Value }
            };
            DRK_ASSERT_DEBUG(noiseTypeMap.contains(spec.type), "Unknown noise type");
            generator.SetNoiseType(noiseTypeMap.at(spec.type));
        }
    }

    Noise::Noise(const Spec& spec, const uvec2& size, int seed)
      : m_spec(spec),
        m_size(size)
    {
        m_data = new float[size.x * size.y];

        FastNoiseLite generator(seed);
        apply_fast_noise_lite_spec(generator, spec);

        for (uint x = 0; x < size.x; ++x)
            for (uint y = 0; y < size.y; ++y)
                m_data[data_index(x, y)] = generator.GetNoise((float)x, (float)y);
    }

    Noise::Noise(Noise&& other)
      : m_spec(other.m_spec),
        m_size(other.m_size),
        m_data(other.m_data)
    {
        other.m_size = {0, 0};
        other.m_data = nullptr;
    }

    Noise& Noise::operator=(Noise&& other)
    {
        delete[] m_data;

        m_spec = other.m_spec;
        m_size = other.m_size;
        m_data = other.m_data;

        other.m_size = {0, 0};
        other.m_data = nullptr;

        return *this;
    }

    Noise::~Noise(void)
    {
        delete[] m_data;
    }

    float Noise::get(const uvec2& point)
    {
        return get(point.x, point.y);
    }

    float Noise::get(uint x, uint y)
    {
        return m_data[data_index(x, y)];
    }

    const Noise::Spec& Noise::info(void) const
    {
        return m_spec;
    }

    uint Noise::data_index(uint x, uint y)
    {
        DRK_ASSERT_DEBUG(x < m_size.x && y < m_size.y, "Index out of range");
        return x + m_size.x * y;
    }
}
