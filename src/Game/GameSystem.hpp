#ifndef DRK_GAME_GAME_SYSTEM_HPP
#define DRK_GAME_GAME_SYSTEM_HPP

#include "Core/Base.hpp"
#include "Util/Timestep.hpp"
#include "Game/World/World.hpp"
#include "Game/Entity/EntityScene.hpp"

namespace DrkCraft
{
    class GameSystem
    {
    public:
        GameSystem(World& world, EntityScene& entityScene);
        virtual ~GameSystem(void) = default;

        virtual void render(void) = 0;
        virtual void update(Timestep timestep) = 0;

    protected:
        World& m_world;
        EntityScene& m_entityScene;
    };
}

#endif // DRK_GAME_GAME_SYSTEM_HPP
