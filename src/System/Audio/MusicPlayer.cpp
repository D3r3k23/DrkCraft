#include "MusicPlayer.hpp"

#include "System/Audio/Audio.hpp"

#include <algorithm>

namespace DrkCraft
{
    MusicPlayer::MusicPlayer(void)
      : m_playing(false),
        m_currentSong(0)
    { }

    void MusicPlayer::add_song(const Ref<AudioSource>& song)
    {
        m_songs.push_back(song);
    }

    void MusicPlayer::remove_song(const Ref<AudioSource>& song)
    {
        if (auto it = std::ranges::find(m_songs, song); it != m_songs.end())
            m_songs.erase(it);
    }

    void MusicPlayer::play(void)
    {

    }

    void MusicPlayer::pause(void)
    {

    }

    void MusicPlayer::stop(void)
    {

    }

    void MusicPlayer::next_song(void)
    {

    }

    void MusicPlayer::shuffle_queue(void)
    {

    }

    void MusicPlayer::clear_queue(void)
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

    bool MusicPlayer::is_playing(void) const
    {
        return m_playing;
    }
}
