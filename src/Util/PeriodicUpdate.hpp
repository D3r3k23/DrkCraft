#ifndef DRK_UTIL_PERIODIC_UPDATE_HPP
#define DRK_UTIL_PERIODIC_UPDATE_HPP

#include "Core/Base.hpp"
#include "Util/Time.hpp"

namespace DrkCraft
{
    class IntervalCounter
    {
    public:
        IntervalCounter(void) = default;

        template <uint INTERVAL>
        bool on_interval(void)
        {
            if (m_count >= INTERVAL)
            {
                m_count = 0;
                return true;
            }
            else
            {
                m_count++;
                return false;
            }
        }

    private:
        uint m_count = 0;
    };

    class IntervalTimer
    {
    public:
        IntervalTimer(void) = default;

        template <uint INTERVAL> // Milliseconds
        bool on_interval(void)
        {
            constexpr Time::Milli<uint> intervalTime(INTERVAL);
            if (m_timer.get_elapsed() >= intervalTime)
            {
                m_timer.reset();
                return true;
            }
            else
                return false;
        }

    private:
        Timer m_timer;
    };
}

#endif // DRK_UTIL_PERIODIC_UPDATE_HPP
