#ifndef DRK_TIMESTEP_HPP
#define DRK_TIMESTEP_HPP

namespace DrkCraft
{
    class Timestep
    {
    public:
        Timestep(void);
        Timestep(const Timestep&) = default;

        float get_seconds(void) const;
        float get_milliseconds(void) const;

        operator double(void) const;
        operator float(void) const;

        static float last_update_time(void);

    private:
        double m_elapsed;

        static double s_lastUpdateTime;
    };
}

#endif // DRK_TIMESTEP_HPP
