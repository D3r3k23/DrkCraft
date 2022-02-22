#ifndef DRK_GAME_SKY_HPP
#define DRK_GAME_SKY_HPP

#include "Core/Base.hpp"
#include "Core/Timestep.hpp"

namespace DrkCraft
{
    class Skybox
    {
    public:
        Skybox(void);

        void render(void);
        void update(Timestep timestep);
    };

    class Sun
    {

    };

    class Moon
    {

    };

    class Sky
    {
    public:
        Sky(void);

        void render(void);
        void update(Timestep timestep);

    private:
        Skybox m_skybox;
        Sun m_sun;
        Moon m_moon;
    };
}

#endif // DRK_GAME_SKY_HPP
