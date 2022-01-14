#ifndef DRK_AUDIO_SOURCE_HPP
#define DRK_AUDIO_SOURCE_HPP

#include "Core/Base.hpp"
#include "AlObject.hpp"

#include <glm/vec3.hpp>

#include <string_view>

namespace DrkCraft
{
    enum class AudioFileFormat
    {
        None = 0,
        Mp3,
        Ogg
    };

    enum class AudioSourceFormat
    {
        None = 0,
        Mono16,
        Stereo16
    };

    AudioFileFormat find_audio_file_format(std::string_view extension);
    AudioSourceFormat get_audio_source_format(uint channels);
    int get_al_source_format(AudioSourceFormat format);

    class AlBuffer : public AlObject
    {
    public:
        AlBuffer(AudioSourceFormat format, int16* data, uint size, uint sampleRate);
        ~AlBuffer(void);

    private:
        AudioSourceFormat m_format;
    };

    class AudioSource : public AlObject
    {
        friend class AudioEngine;

    public:
        AudioSource(AudioSourceFormat format, int16* data, uint size, uint sampleRate, float length);
        ~AudioSource(void);

        bool is_playing(void) const;
        bool is_paused(void) const;
        bool is_stopped(void) const;

        float get_length(void) const;

        void set_position(const glm::vec3& position);
        const glm::vec3& move(const glm::vec3& d);
        const glm::vec3& get_position(void) const;

        void set_spatial(bool spatial);
        void set_gain(float gain);
        void set_pitch(float pitch);
        void set_looping(bool loop);

        bool get_spatial(void) const;
        float get_gain(void) const;
        float get_pitch(void) const;
        bool get_looping(void) const;

    private:
        void attach_buffer(void);
        void detach_buffer(void);

        int get_al_state(void) const;

        void play(void);
        void pause(void);
        void stop(void);
        void rewind(void);

    private:
        AlBuffer m_buffer;

        const float m_length;

        glm::vec3 m_position;
        bool m_spatial;

        float m_gain;
        float m_pitch;
        bool m_loop;
    };
}

#endif // DRK_AUDIO_SOURCE_HPP
