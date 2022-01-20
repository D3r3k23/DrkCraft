#include "AudioSource.hpp"

#include "Core/Profiler.hpp"

#include <AL/al.h>
#include <AL/alext.h>

#include <glm/gtc/type_ptr.hpp>

#include <algorithm>

namespace DrkCraft
{
    AudioFileFormat find_audio_file_format(std::string_view extension)
    {
        if (extension == ".mp3") return AudioFileFormat::Mp3;
        if (extension == ".ogg") return AudioFileFormat::Ogg;
        return AudioFileFormat::None;
    }

    AudioSourceFormat get_audio_source_format(uint channels)
    {
        switch (channels)
        {
            case 1: return AudioSourceFormat::Mono16;
            case 2: return AudioSourceFormat::Stereo16;
            default:
                DRK_ASSERT_DEBUG(false, "Unknown OpenAL format");
                return AudioSourceFormat::None;
        }
    }

    int get_al_source_format(AudioSourceFormat format)
    {
        switch (format)
        {
            case AudioSourceFormat::Mono16   : return static_cast<int>(AL_FORMAT_MONO16);
            case AudioSourceFormat::Stereo16 : return static_cast<int>(AL_FORMAT_STEREO16);
            default:
                DRK_ASSERT_DEBUG(false, "Unknown OpenAL format");
                return 0;
        }
    }

    //////////////////////////
    //       AlBuffer       //
    //////////////////////////

    AlBuffer::AlBuffer(AudioSourceFormat format, int16* data, uint size, uint sampleRate)
    {
        DRK_PROFILE_FUNCTION();

        alGenBuffers(1, &m_id);
        alBufferData(m_id, get_al_source_format(format), data, size, sampleRate);
    }

    AlBuffer::~AlBuffer(void)
    {
        DRK_PROFILE_FUNCTION();
        alDeleteBuffers(1, &m_id);
    }

    /////////////////////////////
    //       AudioSource       //
    /////////////////////////////

    AudioSource::AudioSource(AudioSourceFormat format, int16* data, uint size, uint sampleRate, float length)
      : m_buffer(format, data, size, sampleRate),
        m_length(length)
    {
        DRK_PROFILE_FUNCTION();

        alGenSources(1, &m_id);
        attach_buffer();

        set_spatial(false);
        set_position(glm::vec3(0.0f));
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
        return get_al_state() == AL_PLAYING;
    }

    bool AudioSource::is_paused(void) const
    {
        return get_al_state() == AL_PAUSED;
    }

    bool AudioSource::is_stopped(void) const
    {
        return get_al_state() == AL_STOPPED;
    }

    float AudioSource::get_length(void) const
    {
        return m_length;
    }

    void AudioSource::set_position(const glm::vec3& position)
    {
        m_position = position;
        alSourcefv(m_id, AL_POSITION, glm::value_ptr(m_position));
    }

    const glm::vec3& AudioSource::move(const glm::vec3& d)
    {
        set_position(m_position + d);
        return m_position;
    }

    const glm::vec3& AudioSource::get_position(void) const
    {
        return m_position;
    }

    void AudioSource::set_spatial(bool spatial)
    {
        m_spatial = spatial;
        alSourcei(m_id, AL_SOURCE_SPATIALIZE_SOFT, static_cast<ALint>(spatial));
        alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
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

    bool AudioSource::get_spatial(void) const
    {
        return m_spatial;
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

    void AudioSource::attach_buffer(void)
    {
        alSourcei(m_id, AL_BUFFER, m_buffer.get_id());
    }

    void AudioSource::detach_buffer(void)
    {
        alSourcei(m_id, AL_BUFFER, 0);
    }

    int AudioSource::get_al_state(void) const
    {
        ALenum state;
        alGetSourcei(m_id, AL_SOURCE_STATE, &state);
        return static_cast<int>(state);
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
