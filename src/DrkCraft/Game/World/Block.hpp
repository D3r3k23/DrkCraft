#ifndef DRK_GAME_WORLD_BLOCK_HPP
#define DRK_GAME_WORLD_BLOCK_HPP

#include "Core/Base.hpp"
#include "Graphics/Texture.hpp"

namespace DrkCraft::Game
{
    using BlockID = uint8;
    using ChunkIndex = uint32; // chunk.x + (chunk.z + chunk.y * CHUNK_WIDTH) * CHUNK_WIDTH;

    enum BlockType : BlockID
    {
        VoidBlock  = 0,
        AirBlock   = 1,
        StoneBlock = 2,
        DirtBlock  = 3,
        GrassBlock = 4
    };

    struct Block
    {
        ChunkIndex cid;
        BlockID bid;

        static Ref<Texture> get_texture(BlockID bid);
    };
}

#endif // DRK_GAME_WORLD_BLOCK_HPP
