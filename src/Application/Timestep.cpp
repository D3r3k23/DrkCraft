#include "Timestep.hpp"

#include <GLFW/glfw3.h>

namespace DrkCraft
{
    double Timestep::s_lastUpdateTime = 0;

    Timestep::Timestep(void)
    {
        double now = glfwGetTime();
        m_elapsed = now - s_lastUpdateTime;
        s_lastUpdateTime = now;
    }

    float Timestep::get_seconds(void) const
    {
        return m_elapsed;
    }

    float Timestep::get_milliseconds(void) const
    {
        return m_elapsed * 1000.0;
    }

    Timestep::operator double(void) const
    {
        return get_seconds();
    }

    Timestep::operator float(void) const
    {
        return (float)get_seconds();
    }

    float Timestep::last_update_time(void)
    {
        return s_lastUpdateTime;
    }
}
