#ifndef DRK_TIMER_HPP
#define DRK_TIMER_HPP

#include "Base.hpp"

namespace DrkCraft
{
    class Timer
    {
    public:
        Timer(void);
        void reset(void);

        float elapsed_seconds(void) const;
        float elapsedMilliseconds(void) const;

    private:
        double m_start;
    };
}

#endif // DRK_TIMER_HPP
