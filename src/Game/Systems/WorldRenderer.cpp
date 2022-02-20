#include "WorldRenderer.hpp"

#include "Graphics/Renderer/CubeRenderer.hpp"
#include "Core/Debug/Profiler.hpp"

namespace DrkCraft
{
    WorldRendererSystem::WorldRendererSystem(World& world, EntityScene& entityScene)
      : GameSystem(world, entityScene)
    { }

    void WorldRendererSystem::render(void)
    {
        DRK_PROFILE_FUNCTION();

        CubeRenderer::begin_scene();

        for (const auto& chunks = m_world.get_chunks(); const auto& chunk : chunks)
        {
            render_chunk(chunk);
        }

        CubeRenderer::end_scene();
    }

    void WorldRendererSystem::update(Timestep timestep)
    { }

    void WorldRendererSystem::render_chunk(const Chunk& chunk)
    {
        DRK_PROFILE_FUNCTION();

        for (uint x = 0; x < CHUNK_WIDTH; x++)
            for (uint z = 0; z < CHUNK_WIDTH; z++)
                for (uint y = 0; y < CHUNK_HEIGHT; y++)
                {
                    const auto& block = chunk.block_at(x, y, z);
                    const auto& texture = get_block_texture(block);
                    CubeRenderer::submit({x, y, z}, texture);
                }
    }
}
