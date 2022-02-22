#ifndef DRK_CORE_TIMESTEP_HPP
#define DRK_CORE_TIMESTEP_HPP

#include "Core/Base.hpp"
#include "Util/Time.hpp"

namespace DrkCraft
{
    class Timestep
    {
    public:
        Timestep(float elapsed=0) : m_elapsed(elapsed) {}

        Timestep(const Timestep&) = default;
        Timestep& operator=(Timestep&) = default;

        float elapsed(void)  const { return m_elapsed; }
        operator float(void) const { return m_elapsed; }

    private:
        const float m_elapsed; // Seconds
    };

    class TimestepGenerator
    {
    public:
        TimestepGenerator(void) = default;
        Timestep get_timestep(void);

    private:
        Timer m_timer;
    };
}

#endif // DRK_CORE_TIMESTEP_HPP
