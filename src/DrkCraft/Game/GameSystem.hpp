#ifndef DRK_GAME_GAME_SYSTEM_HPP
#define DRK_GAME_GAME_SYSTEM_HPP

#include "Core/Base.hpp"
#include "Util/Timestep.hpp"
#include "Game/World/World.hpp"
#include "Game/Entity/Scene.hpp"
#include "Game/GameEvent.hpp"

namespace DrkCraft::Game
{
    struct GameSystemData
    {
        World& world;
        Scene& scene;
        GameEventQueue& gameEventQueue;

        GameSystemData(World& world, Scene& scene, GameEventQueue& gameEventQueue)
          : world(world),
            scene(scene),
            gameEventQueue(gameEventQueue)
        { }
    };

    class GameSystem
    {
    public:
        GameSystem(const GameSystemData& data)
          : m_data(data)
        { }

        virtual ~GameSystem(void) = default;

        virtual void render(void) = 0;
        virtual void update(Timestep timestep) = 0;

    protected:
        GameSystemData m_data;
    };
}

#endif // DRK_GAME_GAME_SYSTEM_HPP
