#ifndef DRK_TIMESTEP_HPP
#define DRK_TIMESTEP_HPP

#include "Base.hpp"
#include "Timer.hpp"

namespace DrkCraft
{
    class Timestep
    {
    public:
        Timestep(void);
        Timestep(const Timestep&) = default;

        operator double(void) const;
        operator float(void) const;

    private:
        const double m_elapsed; // Seconds

        static Timer s_frameTimer;
    };
}

#endif // DRK_TIMESTEP_HPP
