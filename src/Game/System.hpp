#ifndef DRK_GAME_SYSTEM_HPP
#define DRK_GAME_SYSTEM_HPP

#include "Core/Base.hpp"
#include "Application/Timestep.hpp"
#include "Game/Entity/EntityScene.hpp"

namespace DrkCraft
{
    class System
    {
    public:
        System(EntityScene& scene);
        virtual ~System(void) = default;

        virtual void render(void) { }
        virtual void update(Timestep timestep) { }

    protected:
        EntityScene& m_scene;
    };
}

#endif // DRK_GAME_SYSTEM_HPP
