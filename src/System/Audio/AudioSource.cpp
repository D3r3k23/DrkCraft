#include "AudioSource.hpp"

#include "Core/Debug/Profiler.hpp"

#include <AL/al.h>
#include <AL/alext.h>

#include <glm/gtc/type_ptr.hpp>

#include <algorithm>

namespace DrkCraft
{
    AudioSource::AudioSource(AudioSourceFormat format, int16* data, uint size, uint sampleRate, uint bitRate)
      : m_buffer(format, data, size, sampleRate),
        m_length(size / (bitRate * 1024.0f))
    {
        DRK_PROFILE_FUNCTION();

        alGenSources(1, &m_id);
        attach_buffer();

        set_spatial(false);
        set_position(vec3(0.0f));
        set_gain(1.0f);
        set_pitch(1.0f);
        set_looping(false);
    }

    AudioSource::~AudioSource(void)
    {
        DRK_PROFILE_FUNCTION();

        detach_buffer();
        alDeleteSources(1, &m_id);
    }

    bool AudioSource::is_playing(void) const
    {
        return get_state() == AudioSourceState::Playing;
    }

    bool AudioSource::is_paused(void) const
    {
        return get_state() == AudioSourceState::Paused;
    }

    bool AudioSource::is_stopped(void) const
    {
        return get_state() == AudioSourceState::Stopped;
    }

    float AudioSource::get_length(void) const
    {
        return m_length;
    }

    void AudioSource::set_gain(float gain)
    {
        m_gain = gain;
        alSourcef(m_id, AL_GAIN, m_gain);
    }

    void AudioSource::set_pitch(float pitch)
    {
        m_pitch = pitch;
        alSourcef(m_id, AL_PITCH, m_pitch);
    }

    void AudioSource::set_looping(bool loop)
    {
        m_loop = loop;
        alSourcei(m_id, AL_LOOPING, static_cast<ALint>(loop));
    }

    void AudioSource::set_spatial(bool spatial)
    {
        m_spatial = spatial;
        alSourcei(m_id, AL_SOURCE_SPATIALIZE_SOFT, static_cast<ALint>(spatial));
        alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
    }

    float AudioSource::get_gain(void) const
    {
        return m_gain;
    }

    float AudioSource::get_pitch(void) const
    {
        return m_pitch;
    }

    bool AudioSource::get_looping(void) const
    {
        return m_loop;
    }

    bool AudioSource::get_spatial(void) const
    {
        return m_spatial;
    }

    void AudioSource::set_position(const vec3& position)
    {
        m_position = position;
        alSourcefv(m_id, AL_POSITION, glm::value_ptr(m_position));
    }

    const vec3& AudioSource::get_position(void) const
    {
        return m_position;
    }

    void AudioSource::attach_buffer(void)
    {
        alSourcei(m_id, AL_BUFFER, m_buffer.get_id());
    }

    void AudioSource::detach_buffer(void)
    {
        alSourcei(m_id, AL_BUFFER, 0);
    }

    AudioSourceState AudioSource::get_state(void) const
    {
        ALenum state;
        alGetSourcei(m_id, AL_SOURCE_STATE, &state);
        switch (state)
        {
            case AL_INITIAL : return AudioSourceState::Initial;
            case AL_PLAYING : return AudioSourceState::Playing;
            case AL_PAUSED  : return AudioSourceState::Paused;
            case AL_STOPPED : return AudioSourceState::Stopped;
            default:
                DRK_ASSERT_DEBUG(false, "Unknown AudioSourceState");
                return AudioSourceState::Initial;
        }
    }

    void AudioSource::play(void)
    {
        DRK_PROFILE_FUNCTION();
        alSourcePlay(m_id);
    }

    void AudioSource::pause(void)
    {
        DRK_PROFILE_FUNCTION();
        if (is_playing())
            alSourcePause(m_id);
    }

    void AudioSource::stop(void)
    {
        DRK_PROFILE_FUNCTION();
        alSourceStop(m_id);
    }

    void AudioSource::rewind(void)
    {
        DRK_PROFILE_FUNCTION();
        alSourceRewind(m_id);
    }
}
