#ifndef DRK_GAME_UTIL_GAME_TIME_HPP
#define DRK_GAME_UTIL_GAME_TIME_HPP

#include "Core/Base.hpp"
#include "Util/Time.hpp"
#include "Util/Timestep.hpp"

namespace DrkCraft::Game
{
    class GameTime
    {
    public:
        GameTime(void);
        GameTime(Time::Seconds<> realTime);
        GameTime(Time::Hours<uint> gameHours);
        GameTime(uint day, uint time=0);

        uint day(void) const;
        uint time(void) const;

        void update(Timestep timestep);

    private:
        static uint to_game_hours(const Time::Seconds<> realTime);
        static Time::Seconds<> to_real_time(uint gameHours);

    private:
        Time::Seconds<> m_realTime;
        uint m_gameHours;
    };
}

#endif // DRK_GAME_UTIL_GAME_TIME_HPP
