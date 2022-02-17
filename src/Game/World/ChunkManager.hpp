#ifndef DRK_GAME_WORLD_CHUNK_MANAGER_HPP
#define DRK_GAME_WORLD_CHUNK_MANAGER_HPP

#include "Core/Base.hpp"
#include "Game/World/Chunk.hpp"
#include "Game/World/ChunkRenderer.hpp"

#include <deque>

namespace DrkCraft
{
    using ChunkContainer = std::deque<Ref<Chunk>>;

    class ChunkManager
    {
    public:


    private:
        ChunkContainer m_Chunks;
    };
}

#endif // DRK_GAME_WORLD_CHUNK_MANAGER_HPP
