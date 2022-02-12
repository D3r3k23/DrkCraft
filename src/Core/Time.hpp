#ifndef DRK_CORE_TIME_HPP
#define DRK_CORE_TIME_HPP

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

        template <typename Rep=double>
        using Seconds = std::chrono::duration<Rep>;

        template <typename Rep=double>
        using Milli = std::chrono::duration<Rep, std::milli>;

        template <typename Rep=double>
        using Micro = std::chrono::duration<Rep, std::micro>;

        using std::chrono::duration_cast;
        using std::chrono::time_point_cast;

        template <typename Duration>
        Duration as_duration(const auto& time)
        {
            return duration_cast<Duration>(time.time_since_epoch());
        }

        Time get_program_start_time(void);
        Time get_program_time(void);

        using SysTime   = std::chrono::sys_time<Seconds<int>>;
        using LocalTime = Seconds<int>; ///

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

#endif // DRK_CORE_TIME_HPP
