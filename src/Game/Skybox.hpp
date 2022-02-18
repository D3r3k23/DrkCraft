#ifndef DRK_GAME_SKYBOX_HPP
#define DRK_GAME_SKYBOX_HPP

#include "Core/Base.hpp"
#include "Application/Timestep.hpp"

namespace DrkCraft
{
    class Skybox
    {
    public:
        Skybox(void);

        void render(void);
        void update(Timestep timestep);
    };
}

#endif // DRK_GAME_SKYBOX_HPP
