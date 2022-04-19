#include "EntityRenderer.hpp"

#include "Graphics/Renderer/MeshRenderer.hpp"
#include "Game/Entity/Components.hpp"
#include "Core/Debug/Profiler.hpp"

namespace DrkCraft
{
    EntityRendererSystem::EntityRendererSystem(World& world, EntityScene& entityScene)
      : GameSystem(world, entityScene)
    { }

    void EntityRendererSystem::render(void)
    {
        DRK_PROFILE_FUNCTION();

        MeshRenderer::begin_scene();

        const auto view = m_entityScene.view<TransformComponent, MeshComponent>();
        view.each([](const auto& tc, const auto& mc)
        {
            MeshRenderer::submit(*(mc.mesh), tc.transform);
        });

        MeshRenderer::end_scene();
    }

    void EntityRendererSystem::update(Timestep timestep)
    {

    }
}
