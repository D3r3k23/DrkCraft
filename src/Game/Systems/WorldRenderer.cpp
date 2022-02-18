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
    }

    void WorldRendererSystem::update(Timestep timestep)
    { }
}
