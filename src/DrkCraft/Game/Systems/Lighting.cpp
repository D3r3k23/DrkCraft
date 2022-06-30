#include "Lighting.hpp"

#include "Core/Debug/Profiler.hpp"

namespace DrkCraft::Game
{
    LightingSystem::LightingSystem(const GameSystemData& data)
      : GameSystem(data)
    { }

    void LightingSystem::render(void)
    {
        DRK_PROFILE_FUNCTION();
    }

    void LightingSystem::update(Timestep timestep)
    {

    }
}
