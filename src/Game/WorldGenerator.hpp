#ifndef WORLD_GAME_GENERATOR_HPP
#define WORLD_GAME_GENERATOR_HPP

#include "Core/Base.hpp"

namespace DrkCraft
{
    class WorldGenerator
    {
    public:
        WorldGenerator(void);

        Ptr<World> generate(void) const;

    private:
    };
}

#endif // WORLD_GAME_GENERATOR_HPP
