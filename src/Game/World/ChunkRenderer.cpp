#include "ChunkRenderer.hpp"

#include "Graphics/Renderer/CubeRenderer.hpp"
#include "Game/World/Block.hpp"

namespace DrkCraft
{
    ChunkRenderer::ChunkRenderer(void)
    {
        CubeRenderer::begin_scene();
    }

    ChunkRenderer::~ChunkRenderer(void)
    {
        CubeRenderer::end_scene();
    }

    void ChunkRenderer::render(const Ref<Chunk>& chunk) const
    {
        for (uint x = 0; x < CHUNK_WIDTH; x++)
            for (uint z = 0; z < CHUNK_WIDTH; z++)
                for (uint y = 0; y < CHUNK_HEIGHT; y++)
                {
                    const auto& block = chunk->block_at(x, y, z);
                    const auto& texture = get_block_texture(block);
                    CubeRenderer::submit({x, y, z}, texture);
                }
    }
}
