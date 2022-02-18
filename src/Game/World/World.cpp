#include "World.hpp"

#include "Game/World/ChunkRenderer.hpp"
#include "Game/Entity/Components.hpp"
#include "Graphics/Renderer/MeshRenderer.hpp"
#include "Core/Debug/Profiler.hpp"

namespace DrkCraft
{
    void World::render(void)
    {
        DRK_PROFILE_FUNCTION();

        render_chunks();
        render_entities();
    }

    void World::render_chunks(void)
    {
        DRK_PROFILE_FUNCTION();

        ChunkRenderer chunkRenderer;
        for (const auto& chunk : m_chunkManager)
            chunkRenderer.render(chunk);
    }

    void World::render_entities(void)
    {
        DRK_PROFILE_FUNCTION();
    }
}
