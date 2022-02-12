#include "Timestep.hpp"

#include <algorithm>

namespace DrkCraft
{
    // If framerate drops below 5 FPS, game will slow down
    const double MAX_TIMESTEP = 1.0 / 5;

    Timer Timestep::s_frameTimer(0.0);

    Timestep::Timestep(void)
      : m_elapsed(std::min(s_frameTimer.elapsed_seconds(), MAX_TIMESTEP))
    {
        s_frameTimer.reset();
    }

    Timestep::operator double(void) const
    {
        return m_elapsed;
    }

    Timestep::operator float(void) const
    {
        return (float)m_elapsed;
    }
}
