#ifndef DRK_CHUNK_MANAGER_HPP
#define DRK_CHUNK_MANAGER_HPP

#include "Core/Base.hpp"
#include "Chunk.hpp"
#include "ChunkRenderer.hpp"

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

#endif // DRK_CHUNK_MANAGER_HPP
