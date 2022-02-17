#ifndef DRK_GAME_WORLD_CHUNK_HPP
#define DRK_GAME_WORLD_CHUNK_HPP

#include "Core/Base.hpp"
#include "Game/World/Block.hpp"

#include "lib/glm/vec3.hpp"

namespace DrkCraft
{
    inline constexpr uint CHUNK_WIDTH  = 64;
    inline constexpr uint CHUNK_HEIGHT = 128;

    class Chunk
    {
    public:
        Chunk(void) = default;

        const Block& block_at(const uvec3& coord);
        const Block& block_at(uint x, uint y, uint z);
    };
}

#endif // DRK_GAME_WORLD_CHUNK_HPP
