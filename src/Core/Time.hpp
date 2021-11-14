#ifndef DRK_TIME_HPP
#define DRK_TIME_HPP

#include <chrono>
#include <ratio>

namespace DrkCraft::Time
{
    using Clock    = std::chrono::steady_clock;
    using Duration = std::chrono::duration<double, std::nano>;
    using Time     = std::chrono::time_point<Clock, Duration>;

    template <typename Rep>
    using Seconds = std::chrono::duration<Rep>;

    template <typename Rep>
    using Milli = std::chrono::duration<Rep, std::milli>;

    template <typename Rep>
    using Micro = std::chrono::duration<Rep, std::micro>;

    using SysTime   = std::chrono::sys_time<Seconds<int>>;
    using LocalTime = std::chrono::local_time<Seconds<int>>;

    using std::chrono::duration_cast;
    using std::chrono::time_point_cast;

    Time get_global_time(void);

    SysTime get_system_time(void);
    LocalTime get_local_time(void);
}

#endif // DRK_TIME_HPP
