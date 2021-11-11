#include "Timestep.hpp"

namespace DrkCraft
{
    Timer Timestep::s_frameTimer(0.0);

    Timestep::Timestep(void)
      : m_elapsed(s_frameTimer.elapsed_seconds())
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
