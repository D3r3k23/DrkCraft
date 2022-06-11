#include "WorldRenderer.hpp"

#include "Graphics/Renderer/BlockRenderer.hpp"
#include "Core/Debug/Profiler.hpp"

namespace DrkCraft::Game
{
    WorldRendererSystem::WorldRendererSystem(GameSystemData data)
      : GameSystem(data)
    { }

    void WorldRendererSystem::render(void)
    {
        DRK_PROFILE_FUNCTION();

        BlockRenderer::begin_scene();

        for (const auto& chunks = m_data.world.get_chunks(); const auto& chunk : chunks)
        {
            render_chunk(chunk);
        }

        BlockRenderer::end_scene();
    }

    void WorldRendererSystem::update(Timestep timestep)
    { }

    void WorldRendererSystem::render_chunk(const Chunk& chunk)
    {
        DRK_PROFILE_FUNCTION();

        for (uint x = 0; x < CHUNK_WIDTH; ++x)
            for (uint z = 0; z < CHUNK_WIDTH; ++z)
                for (uint y = 0; y < CHUNK_HEIGHT; ++y)
                {
                    const auto& block = chunk.block_at(x, y, z);
                    const uint tid = 1; // get_block_tid(block);
                    BlockRenderer::submit({x, y, z}, tid); // , texture);
                }
    }
}
