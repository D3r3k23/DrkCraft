#ifndef DRK_TIMESTEP_HPP
#define DRK_TIMESTEP_HPP

namespace DrkCraft
{
    class Timestep
    {
    public:
        Timestep(void);

        double get_seconds(void) const;
        double get_milliseconds(void) const;

        operator double(void) const;
        operator float(void) const;

        static double last_update_time(void);

    private:
        double m_elapsed;

        static double s_lastUpdateTime;
    };
}

#endif // DRK_TIMESTEP_HPP
