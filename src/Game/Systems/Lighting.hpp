#ifndef DRK_GAME_SYSTEMS_LIGHTING_HPP
#define DRK_GAME_SYSTEMS_LIGHTING_HPP

#include "Core/Base.hpp"
#include "Game/GameSystem.hpp"
#include "Util/Timestep.hpp"
#include "Game/World/World.hpp"
#include "Game/Entity/EntityScene.hpp"

namespace DrkCraft::Game
{
    class LightingSystem : public GameSystem
    {
    public:
        LightingSystem(World& world, EntityScene& scene);
        virtual ~LightingSystem(void) = default;

        virtual void render(void) override;
        virtual void update(Timestep timestep) override;
    };
}

#endif // DRK_GAME_SYSTEMS_LIGHTING_HPP
