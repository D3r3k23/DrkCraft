#ifndef DRK_TIMER_HPP
#define DRK_TIMER_HPP

#include "Base.hpp"

#include <chrono>

namespace DrkCraft
{
    using TimerClock    = std::chrono::steady_clock;
    using TimerDuration = std::chrono::duration<double>;

    class Timer
    {
    public:
        static TimerDuration get_global_time(void);

        Timer(void);
        Timer(double init);

        void reset(void);

        TimerDuration get_elapsed(void) const;
        TimerDuration get_start(void) const;

        double elapsed_seconds(void) const;
        double elapsed_milliseconds(void) const;
        double elapsed_microseconds(void) const;

    private:
        static std::chrono::time_point<TimerClock> s_globalStart;

        TimerDuration m_start;
    };
}

#endif // DRK_TIMER_HPP
