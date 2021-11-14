#include "Time.hpp"

namespace DrkCraft::Time
{
    static Time s_globalStart = Clock::now();

    Time get_global_time(void)
    {
        return Time(duration_cast<Duration>(Clock::now() - s_globalStart));
    }

    SysTime get_system_time(void)
    {
        return time_point_cast<Seconds<int>>(std::chrono::system_clock::now());
    }

    LocalTime get_local_time(void)
    {
        return time_point_cast<Seconds<int>>(std::chrono::current_zone()->to_local(get_system_time()));
    }
}
