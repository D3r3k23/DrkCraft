#include "Timer.hpp"

namespace DrkCraft
{
    Timer::Timer(void)
    {
        reset();
    }

    Timer::Timer(double init)
    {
        m_start = Time::Time(Time::Duration(init * 1000));
    }

    void Timer::reset(void)
    {
        m_start = Time::get_global_time();
    }

    Time::Duration Timer::get_elapsed(void) const
    {
        return Time::duration_cast<Time::Duration>(Time::get_global_time() - m_start);
    }

    Time::Time Timer::get_start(void) const
    {
        return m_start;
    }

    double Timer::elapsed_seconds(void) const
    {
        Time::Duration elapsed = get_elapsed();
        return duration_cast<Time::Seconds<double>>(elapsed).count();
    }

    double Timer::elapsed_milliseconds(void) const
    {
        Time::Duration elapsed = get_elapsed();
        return Time::duration_cast<Time::Milli<double>>(elapsed).count();
    }

    double Timer::elapsed_microseconds(void) const
    {
        Time::Duration elapsed = get_elapsed();
        return Time::duration_cast<Time::Micro<double>>(elapsed).count();
    }
}
