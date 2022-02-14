#ifndef DRK_AUDIO_AUDIO_SOURCE_HPP
#define DRK_AUDIO_AUDIO_SOURCE_HPP

#include "Core/Base.hpp"
#include "Audio/detail/AlObject.hpp"
#include "Audio/detail/Buffer.hpp"
#include "Audio/detail/Util.hpp"

#include "lib/glm/vec3.hpp"

#include <functional>

namespace DrkCraft
{
    template <typename B>
    using AudioSourceBufferDeleter = std::function<void(B*)>;

    template <typename B>
    struct AudioSourceData
    {
        AudioSourceData(B* buffer, const AudioSourceBufferDeleter<B>& bufferDeleter={})
          : buffer(buffer),
            m_deleter(bufferDeleter)
        { }

        ~AudioSourceData(void)
        {
            if (m_deleter)
                m_deleter(buffer);
        }

        // Bitrate could be variable - do we need to find it?
        // Might be required to get AudioSourceFormat

        B* buffer;
        uint size=0;
        uint channels=0;
        uint sampleRate=0;
        uint bitRate=0; // kb/s
        uint bitDepth=0;
        AudioSourceFormat format=AudioSourceFormat::None;

    private:
        AudioSourceBufferDeleter<B> m_deleter;
    };

    enum class AudioSourceState
    {
        Initial = 0,
        Paused,
        Playing,
        Stopped
    };

    class AudioSource : public AlObject
    {
        friend class AudioEngine;

    private:
        AudioSource(AudioSourceFormat format, void* data, uint size, uint sampleRate, uint bitRate);

    public:
        template <typename B>
        AudioSource(const AudioSourceData<B>& data)
          : AudioSource(data.format, static_cast<void*>data.buffer, data.size, data.sampleRate, data.bitRate)
        { }

        virtual ~AudioSource(void);

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

        AudioSourceState get_state(void) const;

        void play(void);
        void pause(void);
        void stop(void);
        void rewind(void);

    private:
        AlBuffer m_buffer;

        const float m_length; // Seconds

        vec3 m_position;
        bool m_spatial;

        float m_gain;
        float m_pitch;
        bool m_loop;
    };
}

#endif // DRK_AUDIO_AUDIO_SOURCE_HPP
