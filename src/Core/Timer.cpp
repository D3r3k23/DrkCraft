#include "Timer.hpp"

#include <GLFW/glfw3.h>

namespace DrkCraft
{
    Timer::Timer(void)
    {
        reset();
    }

    void Timer::reset(void)
    {
        m_start = glfwGetTime();
    }

    float Timer::elapsed_seconds(void) const
    {
        return glfwGetTime() - m_start;
    }

    float Timer::elapsedMilliseconds(void) const
    {
        return elapsed_seconds() * 1000.0f;
    }
}
