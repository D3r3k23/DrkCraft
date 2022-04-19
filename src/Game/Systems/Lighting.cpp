#include "Lighting.hpp"

#include "Core/Debug/Profiler.hpp"

namespace DrkCraft
{
    LightingSystem::LightingSystem(World& world, EntityScene& entityScene)
      : GameSystem(world, entityScene)
    { }

    void LightingSystem::render(void)
    {
        DRK_PROFILE_FUNCTION();
    }

    void LightingSystem::update(Timestep timestep)
    {

    }
}
