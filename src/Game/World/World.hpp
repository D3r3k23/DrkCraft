#ifndef DRK_GAME_WORLD_WORLD_HPP
#define DRK_GAME_WORLD_WORLD_HPP

#include "Core/Base.hpp"
#include "Game/World/Chunk.hpp"
#include "Game/World/WorldGenerator.hpp"

#include <vector>

namespace DrkCraft
{
    struct World
    {
    public:
        World(void) = default;

        const std::vector<Chunk>& get_chunks(void) const;

    private:
        std::vector<Chunk> m_chunks;
        WorldGenerator generator;
    };
}

#endif // DRK_GAME_WORLD_WORLD_HPP
