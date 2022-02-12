#ifndef DRK_SYSTEM_AUDIO_SPATIAL_HPP
#define DRK_SYSTEM_AUDIO_SPATIAL_HPP

#include "Core/Base.hpp"
#include "System/Audio/AudioSource.hpp"
#include "Core/Util.hpp"

namespace DrkCraft
{
    class SpatialAudioController
    {
    public:
        SpatialAudioController(Ref<AudioSource> source, Direction direction=Direction::None, float distance=1.0f);

        void set_position(Direction direction, float distance);

        void set_direction(Direction direction);
        void set_distance(float distance);

        Direction get_direction(void) const;
        float get_distance(void) const;

    private:
        void update_position(void);

    private:
        Ref<AudioSource> m_source;

        Direction m_direction;
        float m_distance;
    };
}

#endif // DRK_SYSTEM_AUDIO_SPATIAL_HPP
