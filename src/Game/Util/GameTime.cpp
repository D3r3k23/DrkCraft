#include "GameTime.hpp"

namespace DrkCraft::Game
{
    static constexpr uint HOURS_PER_DAY = 10;
    static constexpr uint SECONDS_PER_HOUR = 360;
    static constexpr uint SECONDS_PER_DAY = SECONDS_PER_HOUR * HOURS_PER_DAY;
    //        Game       //                  Real                 //
    // 24 days // 240 hr // 86400 s // 1440 min // 24 hr // 1 day //
    //  1 day  // 10 hr  // 3600 s  //  60 min  // 1 hr  //       //
    //         // 1 hr   // 360 s   //  10 min  //       //       //
    //         //        // 60 s    //  1 min   //       //       //

    GameTime::GameTime(void)
      : m_realTime(0.0),
        m_gameHours(0)
    { }

    GameTime::GameTime(Time::Seconds<> realTime)
      : m_realTime(realTime),
        m_gameHours(to_game_hours(realTime))
    { }

    GameTime::GameTime(Time::Hours<uint> gameHours)
      : GameTime(to_real_time(gameHours.count()))
    { }

    GameTime::GameTime(uint day, uint time)
      : GameTime(to_real_time(day * HOURS_PER_DAY + time))
    { }

    uint GameTime::day(void) const
    {
        return m_gameHours / HOURS_PER_DAY;
    }

    uint GameTime::time(void) const
    {
        return m_gameHours % HOURS_PER_DAY;
    }

    void GameTime::update(Timestep timestep)
    {
        m_realTime += timestep.seconds();
        m_gameHours = to_game_hours(m_realTime);
    }

    uint GameTime::to_game_hours(const Time::Seconds<> realTime)
    {
        return realTime.count() / 3600;
    }

    Time::Seconds<> GameTime::to_real_time(uint gameHours)
    {
        return Time::Seconds<>(gameHours);
    }
}
