#ifndef DRK_GAME_WORLD_WORLD_GENERATOR_HPP
#define DRK_GAME_WORLD_WORLD_GENERATOR_HPP

#include "Core/Base.hpp"
#include "Game/World/World.hpp"
#include "Util/Noise.hpp"

namespace DrkCraft
{
    struct WorldGeneratorSpec
    {

    };

    class WorldGenerator
    {
    public:
        WorldGenerator(const WorldGeneratorSpec& spec);
        Ptr<World> generate(void);

        const WorldGeneratorSpec& info(void) const;

    private:

    private:
        const WorldGeneratorSpec m_spec;
        Ptr<World> m_world;

        Noise noiseMap;
    };
}

#endif // DRK_GAME_WORLD_WORLD_GENERATOR_HPP
