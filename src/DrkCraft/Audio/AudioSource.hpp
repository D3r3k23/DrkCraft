#ifndef DRK_AUDIO_AUDIO_SOURCE_HPP
#define DRK_AUDIO_AUDIO_SOURCE_HPP

#include "Core/Base.hpp"
#include "Audio/detail/AlObject.hpp"
#include "Audio/detail/Buffer.hpp"
#include "Audio/detail/Util.hpp"

#include "lib/glm/vec3.hpp"

namespace DrkCraft
{
    enum class AudioSourceState
    {
        Initial = 0,
        Paused,
        Playing,
        Stopped
    };

    class AudioSource : public AlObject
    {
    public:
        class Passkey
        {
            friend class AudioEngine;
        private:
            Passkey(void) = default;
        };

        AudioSource(AudioSourceFormat format, const void* data, uint size, uint sampleRate, float length);

        virtual ~AudioSource(void);

        void play(Passkey);
        void pause(Passkey);
        void stop(Passkey);
        void rewind(Passkey);

        AudioSourceState get_state(void) const;

        bool is_playing(void) const;
        bool is_paused(void) const;
        bool is_stopped(void) const;

        float get_length(void) const;

        void set_gain(float gain);
        void set_pitch(float pitch);
        void set_looping(bool loop);
        void set_spatial(bool spatial);

        float get_gain(void) const;
        float get_pitch(void) const;
        bool get_looping(void) const;
        bool get_spatial(void) const;

        void set_position(const vec3& position);
        const vec3& get_position(void) const;

    private:
        void attach_buffer(void);
        void detach_buffer(void);

    private:
        AlBuffer m_buffer;

        const float m_length;

        vec3 m_position;
        bool m_spatial;

        float m_gain;
        float m_pitch;
        bool m_loop;
    };
}

#endif // DRK_AUDIO_AUDIO_SOURCE_HPP
