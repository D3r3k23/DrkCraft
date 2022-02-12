#ifndef DRK_GAME_CHUNK_RENDERER_HPP
#define DRK_GAME_CHUNK_RENDERER_HPP

#include "Core/Base.hpp"
#include "Game/Chunk.hpp"

namespace DrkCraft
{
    class ChunkRenderer
    {
    public:
        ChunkRenderer(void);
        ~ChunkRenderer(void);

        void render(const Ref<Chunk>& chunk) const;

    private:
    };
}

#endif // DRK_GAME_CHUNK_RENDERER_HPP
