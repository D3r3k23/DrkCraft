#include "Spatial.hpp"

namespace DrkCraft
{
    SpatialAudioController::SpatialAudioController(Ref<AudioSource> source, Direction direction=Direction::None, float distance=1.0f)
      : m_source(std::move(source))
    {
        m_source->set_spatial(true);
        set_position(direction, distance);
    }

    void SpatialAudioController::set_position(Direction direction, float distance)
    {
        m_direction = direction;
        m_distance  = distance;
        update_position();
    }

    void SpatialAudioController::set_direction(Direction direction)
    {
        m_direction = direction;
        update_position();
    }

    void SpatialAudioController::set_distance(float distance)
    {
        m_distance = distance;
        update_position();
    }

    Direction SpatialAudioController::get_direction(void) const
    {
        return m_direction;
    }

    float SpatialAudioController::get_distance(void) const
    {
        return m_distance;
    }

    void SpatialAudioController::update_position(void)
    {
        m_source->set_position(m_distance * unit_vector(m_direction));
    }
}
