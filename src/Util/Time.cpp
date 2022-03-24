#include "Time.hpp"

namespace DrkCraft
{
    namespace Time
    {
        static Time s_programStart = Clock::now();
        static const auto s_currentTimezone = std::chrono::current_zone();

        Time get_program_start_time(void)
        {
            return s_programStart;
        }

        Time get_program_time(void)
        {
            return Time(duration_cast<Duration>(Clock::now() - s_programStart));
        }

        SysTime get_system_time(void)
        {
            return time_point_cast<Seconds<int>>(std::chrono::system_clock::now());
        }

        LocalTime get_local_time(void)
        {
            return s_currentTimezone->to_local(get_system_time());
        }
    }

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
        m_start = Time::get_program_time();
    }

    Time::Duration Timer::get_elapsed(void) const
    {
        return Time::get_program_time() - m_start;
    }

    Time::Time Timer::get_start(void) const
    {
        return m_start;
    }

    double Timer::elapsed_seconds(void) const
    {
        Time::Duration elapsed = get_elapsed();
        return Time::Seconds<double>(elapsed).count();
    }

    double Timer::elapsed_milliseconds(void) const
    {
        Time::Duration elapsed = get_elapsed();
        return Time::Milli<double>(elapsed).count();
    }

    double Timer::elapsed_microseconds(void) const
    {
        Time::Duration elapsed = get_elapsed();
        return Time::Micro<double>(elapsed).count();
    }
}
