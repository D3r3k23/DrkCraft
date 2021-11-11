#include "Timer.hpp"

namespace DrkCraft
{
    std::chrono::time_point<TimerClock> Timer::s_globalStart = TimerClock::now();

    TimerDuration Timer::get_global_time(void)
    {
        return TimerClock::now() - s_globalStart;
    }

    Timer::Timer(void)
    {
        reset();
    }

    Timer::Timer(double init)
    {
        m_start = TimerDuration(init);
    }

    void Timer::reset(void)
    {
        m_start = get_global_time();
    }

    TimerDuration Timer::get_elapsed(void) const
    {
        return get_global_time() - m_start;
    }

    TimerDuration Timer::get_start(void) const
    {
        return m_start;
    }

    double Timer::elapsed_seconds(void) const
    {
        TimerDuration elapsed = get_elapsed();
        return elapsed.count();
    }

    double Timer::elapsed_milliseconds(void) const
    {
        TimerDuration elapsed = get_elapsed();
        return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    }

    double Timer::elapsed_microseconds(void) const
    {
        TimerDuration elapsed = get_elapsed();
        return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    }
}
