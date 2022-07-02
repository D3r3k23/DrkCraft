#ifndef DRK_AUDIO_IPOD_HPP
#define DRK_AUDIO_IPOD_HPP

#include "Core/Base.hpp"
#include "Audio/AudioSource.hpp"

#include <deque>

namespace DrkCraft
{
    class iPod
    {
    public:
        iPod(void);

        void add_song(const Ref<AudioSource>& song);
        void remove_song(const Ref<AudioSource>& song);

        void play(void);
        void pause(void);

        void stop(void);
        void next_song(void);

        void shuffle_queue(void);
        void clear_queue(void);

        bool is_playing(void) const;

    private:
        std::deque<Ref<AudioSource>> m_songs;
        bool m_playing;
        uint m_currentSong;
    };
}

#endif // DRK_AUDIO_IPOD_HPP
