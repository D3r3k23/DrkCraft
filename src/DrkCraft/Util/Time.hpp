#ifndef DRK_UTIL_TIME_HPP
#define DRK_UTIL_TIME_HPP

#include "Core/Base.hpp"

#include <chrono>
#include <ratio>

namespace DrkCraft
{
    namespace Time
    {
        using Clock    = std::chrono::steady_clock;
        using Duration = std::chrono::duration<double, std::nano>; // micro?
        using Time     = std::chrono::time_point<Clock, Duration>;

        using MinutePeriod    = std::ratio<60>;
        using HourPeriod   = std::ratio<60*60>;
        using DayPeriod = std::ratio<24*60*60>;

        template <typename Rep=double>
        using Micro = std::chrono::duration<Rep, std::micro>;

        template <typename Rep=double>
        using Milli = std::chrono::duration<Rep, std::milli>;

        template <typename Rep=double>
        using Seconds = std::chrono::duration<Rep>;

        template <typename Rep=double>
        using Minutes = std::chrono::duration<Rep, MinutePeriod>;

        template <typename Rep=double>
        using Hours = std::chrono::duration<Rep, HourPeriod>;

        template <typename Rep=double>
        using Days = std::chrono::duration<Rep, DayPeriod>;

        using std::chrono::duration_cast;
        using std::chrono::time_point_cast;

        template <typename D=Duration>
        D as_duration(const Time& time)
        {
            return duration_cast<D>(time.time_since_epoch());
        }

        Time get_program_start_time(void);
        Time get_program_time(void);

        using SysTime   = std::chrono::sys_time<Seconds<int>>;
        using LocalTime = std::chrono::local_time<Seconds<int>>;

        SysTime get_system_time(void);
        LocalTime get_local_time(void);
    }

    class Timer
    {
    public:
        Timer(void);
        Timer(double init); // Seconds

        void reset(void);

        // stop(), pause()?

        Time::Duration get_elapsed(void) const;
        Time::Time get_start(void) const;

        double elapsed_seconds(void) const;
        double elapsed_milliseconds(void) const;
        double elapsed_microseconds(void) const;

    private:
        Time::Time m_start;
    };
}

#endif // DRK_UTIL_TIME_HPP
