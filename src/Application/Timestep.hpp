#ifndef DRK_APPLICATION_TIMESTEP_HPP
#define DRK_APPLICATION_TIMESTEP_HPP

#include "Core/Base.hpp"
#include "Util/Time.hpp"

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

#endif // DRK_APPLICATION_TIMESTEP_HPP
