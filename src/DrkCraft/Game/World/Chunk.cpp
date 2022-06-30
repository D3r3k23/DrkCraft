#include "Chunk.hpp"

namespace DrkCraft::Game
{
    const Block& Chunk::block_at(const uvec3& coord) const
    {
        return block_at(coord.x, coord.y, coord.z);
    }

    const Block& Chunk::block_at(uint x, uint y, uint z) const
    {
        return {};
    }
}
