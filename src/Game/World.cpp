#include "World.hpp"

#include "Game/ChunkRenderer.hpp"
#include "Graphics/MeshRenderer.hpp"
#include "Core/Debug/Profiler.hpp"

namespace DrkCraft
{
    Ptr<World> World::load_save(const fs::path& saveDir)
    {
        return {};
    }

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

        MeshRenderer::begin_scene();
        EntityRegistry& entities = m_entityManager.get_entities();
        auto group = entities.group<TransformComponent>(entt::get<MeshComponent>);

        for (auto entity : group)
        {
            auto [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
            MeshRenderer::submit(mesh, transform);
        }
        MeshRenderer::end_scene();
    }
}
