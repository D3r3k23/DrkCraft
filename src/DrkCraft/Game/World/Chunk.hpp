#ifndef DRK_GAME_WORLD_CHUNK_HPP
#define DRK_GAME_WORLD_CHUNK_HPP

#include "Core/Base.hpp"
#include "Game/World/Block.hpp"

#include "Lib/glm/vec3.hpp"

namespace DrkCraft::Game
{
    inline constexpr uint CHUNK_WIDTH  = 64;
    inline constexpr uint CHUNK_HEIGHT = 128;

    class Chunk
    {
    public:
        Chunk(void) = default;

        const Block& block_at(const uvec3& coord) const;
        const Block& block_at(uint x, uint y, uint z) const;
    };
}

#endif // DRK_GAME_WORLD_CHUNK_HPP
