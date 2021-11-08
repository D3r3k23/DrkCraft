#ifndef DRK_TIMER_HPP
#define DRK_TIMER_HPP

namespace DrkCraft
{
    class Timer
    {
    public:
        Timer(void);
        void reset(void);

        float elapsed_seconds(void);
        float elapsedMilliseconds(void);

    private:
        double m_start;
    };
}

#endif // DRK_TIMER_HPP
