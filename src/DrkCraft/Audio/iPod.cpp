#include "iPod.hpp"

#include "Audio/Audio.hpp"

#include <algorithm>

namespace DrkCraft
{
    iPod::iPod(void)
      : m_playing(false),
        m_currentSong(0)
    { }

    void iPod::add_song(const Ref<AudioSource>& song)
    {
        m_songs.push_back(song);
    }

    void iPod::remove_song(const Ref<AudioSource>& song)
    {
        if (auto it = std::ranges::find(m_songs, song); it != m_songs.end())
            m_songs.erase(it);
    }

    void iPod::play(void)
    {

    }

    void iPod::pause(void)
    {

    }

    void iPod::stop(void)
    {

    }

    void iPod::next_song(void)
    {

    }

    void iPod::shuffle_queue(void)
    {

    }

    void iPod::clear_queue(void)
    {
        Ref<AudioSource> playingSong;
        if (is_playing())
            playingSong = m_songs[m_currentSong];

        m_songs.clear();
        m_currentSong = 0;
        if (is_playing())
        {
            m_songs.push_back(playingSong);
        }
    }

    bool iPod::is_playing(void) const
    {
        return m_playing;
    }
}
