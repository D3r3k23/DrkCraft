#ifndef DRK_GAME_SKY_HPP
#define DRK_GAME_SKY_HPP

#include "Core/Base.hpp"
#include "Game/Util/GameTime.hpp"

namespace DrkCraft::Game
{
    class Skybox
    {
    public:
        Skybox(void);

        void render(void);
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
        void update(const GameTime& gameTime);

    private:
        Skybox m_skybox;
        // Sun m_sun;
        // Moon m_moon;

        uint m_hour;
    };
}

#endif // DRK_GAME_SKY_HPP
