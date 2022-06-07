#ifndef DRK_UTIL_TIMESTEP_HPP
#define DRK_UTIL_TIMESTEP_HPP

#include "Core/Base.hpp"
#include "Util/Time.hpp"

namespace DrkCraft
{
    class Timestep
    {
    public:
        Timestep(float ts=0) : m_delta(ts) {}

        Timestep(const Timestep&) = default;
        Timestep& operator=(Timestep&) = default;

        float ts(void)   const { return m_delta; }
        float delta(void)  const { return m_delta; }
        operator float(void) const { return m_delta; }

    private:
        const float m_delta; // Seconds
    };

    // If framerate drops below 5 FPS, game will slow down
    static constexpr float MAX_TIMESTEP = 1.0 / 5;

    class TimestepGenerator
    {
    public:
        TimestepGenerator(void) = default;

        Timestep get_timestep(void)
        {
            float elapsed = m_timer.elapsed_seconds();
            m_timer.reset();
            return { elapsed < MAX_TIMESTEP ? elapsed : MAX_TIMESTEP };
        }

    private:
        Timer m_timer;
    };
}

#endif // DRK_UTIL_TIMESTEP_HPP
