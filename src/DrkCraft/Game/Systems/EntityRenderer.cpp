#include "EntityRenderer.hpp"

#include "Graphics/Renderer/MeshRenderer.hpp"
#include "Game/Entity/Components.hpp"
#include "Core/Debug/Profiler.hpp"

namespace DrkCraft::Game
{
    EntityRendererSystem::EntityRendererSystem(const GameSystemData& data)
      : GameSystem(data)
    { }

    void EntityRendererSystem::render(void)
    {
        DRK_PROFILE_FUNCTION();

        MeshRenderer::begin_scene();

        const auto view = m_data.scene.view<TransformComponent, MeshComponent>();
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
