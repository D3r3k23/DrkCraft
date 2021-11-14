#ifndef DRK_TIMER_HPP
#define DRK_TIMER_HPP

#include "Base.hpp"
#include "Time.hpp"

namespace DrkCraft
{
    class Timer
    {
    public:
        Timer(void);
        Timer(double init); // Seconds

        void reset(void);

        Time::Duration get_elapsed(void) const;
        Time::Time get_start(void) const;

        double elapsed_seconds(void) const;
        double elapsed_milliseconds(void) const;
        double elapsed_microseconds(void) const;

    private:
        Time::Time m_start;
    };
}

#endif // DRK_TIMER_HPP
