#ifndef WORLD_GAME_WORLD_GENERATOR_HPP
#define WORLD_GAME_WORLD_GENERATOR_HPP

#include "Core/Base.hpp"
#include "Game/World.hpp"

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
    };
}

#endif // WORLD_GAME_WORLD_GENERATOR_HPP
