#include "WorldGenerator.hpp"

#include "Core/Debug/Profiler.hpp"

#include <FastNoiseLite.h>

#include <utility>

namespace DrkCraft
{
    WorldGenerator::WorldGenerator(const WorldGeneratorSpec& spec)
      : m_spec(spec)
    {

    }

    Ptr<World> WorldGenerator::generate(void)
    {
        DRK_PROFILE_FUNCTION();

        m_world = make_ptr<World>();

        return std::move(m_world);
    }

    const WorldGeneratorSpec& WorldGenerator::info(void) const
    {
        return m_spec;
    }
}
