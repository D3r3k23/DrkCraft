#include "Timestep.hpp"

#include <algorithm>

namespace DrkCraft
{
    // If framerate drops below 5 FPS, game will slow down
    static constexpr double MAX_TIMESTEP = 1.0 / 5;

    Timestep TimestepGenerator::get_timestep(void)
    {
        Timestep timestep(static_cast<float>(std::min(m_timer.elapsed_seconds(), MAX_TIMESTEP)));
        m_timer.reset();
        return timestep;
    }
}
