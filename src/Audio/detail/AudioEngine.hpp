#ifndef DRK_AUDIO_DETAIL_AUDIO_ENGINE_HPP
#define DRK_AUDIO_DETAIL_AUDIO_ENGINE_HPP

#include "Core/Base.hpp"
#include "Audio/AudioSource.hpp"
#include "Audio/detail/AlTools.hpp"
#include "Audio/detail/Decoder.hpp"

#include "lib/fs.hpp"

#include <deque>

namespace DrkCraft
{
    // Loading is thread safe, playing/control is not
    class AudioEngine
    {
        friend class Audio;

    private:
        AudioEngine(void);
    public:
        ~AudioEngine(void);

    private:
        Ref<AudioSource> load_source(const fs::path& filename, AudioFileFormat format=AudioFileFormat::None);

        void play_source(const Ref<AudioSource>& source);
        void unpause_source(const Ref<AudioSource>& source);
        void unpause_all(void);

        void pause_source(const Ref<AudioSource>& source);
        void pause_all(void);

        void stop_source(const Ref<AudioSource>& source);
        void stop_all(void);

        void set_gain(float gain);

        void refresh(void);

        const AlDevice& get_device(void) const;
        const AlContext& get_context(void) const;

    private:
        Ptr<AlDevice>  m_device;
        Ptr<AlContext> m_context;

        std::deque<Ref<AudioSource>> m_playingSources;

        Mp3Decoder m_mp3Decoder;
    };
}

#endif // DRK_AUDIO_DETAIL_AUDIO_ENGINE_HPP
