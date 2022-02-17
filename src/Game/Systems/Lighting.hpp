#ifndef DRK_GAME_SYSTEMS_LIGHTING_HPP
#define DRK_GAME_SYSTEMS_LIGHTING_HPP

#include "Core/Base.hpp"
#include "Game/System.hpp"
#include "Application/Timestep.hpp"
#include "Game/Entity/EntityScene.hpp"

namespace DrkCraft
{
    class LightingSystem : public System
    {
    public:
        LightingSystem(EntityScene& scene);
        ~LightingSystem(void) = default;
    };
}

#endif // DRK_GAME_SYSTEMS_LIGHTING_HPP
