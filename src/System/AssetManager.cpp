#include "AssetManager.hpp"

#include "Audio/Audio.hpp"
#include <Core/Time.hpp>
#include "Core/Profiler.hpp"

#include <algorithm>

namespace DrkCraft
{
    using std::filesystem::path;

    static const path ASSET_DIR = "assets";
    static const path FONT_ASSET_DIR    = ASSET_DIR / "fonts";
    static const path ICON_ASSET_DIR    = ASSET_DIR / "icons";
    static const path IMAGE_ASSET_DIR   = ASSET_DIR / "images";
    static const path MODEL_ASSET_DIR   = ASSET_DIR / "models";
    static const path MUSIC_ASSET_DIR   = ASSET_DIR / "music";
    static const path SHADER_ASSET_DIR  = ASSET_DIR / "shaders";
    static const path SOUND_ASSET_DIR   = ASSET_DIR / "sounds";
    static const path TEXTURE_ASSET_DIR = ASSET_DIR / "textures";

    path font_asset_path(const path& filename)    { return FONT_ASSET_DIR    / filename; }
    path icon_asset_path(const path& filename)    { return ICON_ASSET_DIR    / filename; }
    path image_asset_path(const path& filename)   { return IMAGE_ASSET_DIR   / filename; }
    path model_asset_path(const path& filename)   { return MODEL_ASSET_DIR   / filename; }
    path music_asset_path(const path& filename)   { return MUSIC_ASSET_DIR   / filename; }
    path shader_asset_path(const path& filename)  { return SHADER_ASSET_DIR  / filename; }
    path sound_asset_path(const path& filename)   { return SOUND_ASSET_DIR   / filename; }
    path texture_asset_path(const path& filename) { return TEXTURE_ASSET_DIR / filename; }

    ////////////////////////////////
    //       AssetLoadQueue       //
    ////////////////////////////////

    void AssetManager::AssetLoadQueue::push(const AssetInfo& asset)
    {
        std::lock_guard lock(m_queueMutex);
        m_queue.push(asset);
    }

    void AssetManager::AssetLoadQueue::push(const AssetList& assets)
    {
        std::lock_guard lock(m_queueMutex);
        for (const auto asset : assets)
            m_queue.push(asset);
    }

    AssetInfo AssetManager::AssetLoadQueue::pop(void)
    {
        DRK_PROFILE_FUNCTION();

        std::lock_guard lock(m_queueMutex);
        if (m_queue.empty())
            return {};

        const auto front = m_queue.front();
        m_queue.pop();
        return front;
    }

    bool AssetManager::AssetLoadQueue::empty(void)
    {
        std::lock_guard lock(m_queueMutex);
        return m_queue.empty();
    }

    //////////////////////////////
    //       AssetManager       //
    //////////////////////////////

    AssetManager::AssetManager(void)
      : m_working(true),
        m_loading(false)
    {
        DRK_PROFILE_FUNCTION();
        m_loadThread = std::thread(DRK_BIND_FN(load_worker));
    }

    AssetManager::~AssetManager(void)
    {
        stop_loading();
    }

    void AssetManager::stop_loading(void)
    {
        m_working = false;
        m_loadThread.join();
    }

    void AssetManager::unload_all(void)
    {
        std::scoped_lock lock(m_imagesMutex, m_soundsMutex, m_songsMutex, m_fontsMutex);
        m_images.clear();
        m_sounds.clear();
        m_songs.clear();
        m_fonts.clear();
    }

    void AssetManager::load(const AssetInfo& asset)
    {
        m_loadQueue.push(asset);
    }

    void AssetManager::load_list(const AssetList& assets)
    {
        m_loadQueue.push(assets);
    }

    bool AssetManager::sound_loaded(const std::filesystem::path& filename)
    {
        std::lock_guard lock(m_soundsMutex);
        return m_sounds.contains(filename.string());
    }

    bool AssetManager::song_loaded(const std::filesystem::path& filename)
    {
        std::lock_guard lock(m_songsMutex);
        return m_songs.contains(filename.string());
    }

    Ref<AudioSource> AssetManager::get_sound(const std::filesystem::path& filename)
    {
        std::lock_guard lock(m_soundsMutex);
        DRK_ASSERT_DEBUG(m_sounds.contains(filename.string()), "Sound not loaded");
        return m_sounds[filename.string()];
    }

    Ref<AudioSource> AssetManager::get_song(const std::filesystem::path& filename)
    {
        std::lock_guard lock(m_songsMutex);
        DRK_ASSERT_CORE(m_songs.contains(filename.string()), "Song not loaded");
        return m_songs[filename.string()];
    }

    void AssetManager::unload(const AssetInfo& asset)
    {
        DRK_PROFILE_FUNCTION();

        const auto& [type, filename] = asset;
        switch (type)
        {
            case AssetType::Image:
            {
                std::lock_guard lock(m_imagesMutex);
                if (auto it = m_images.find(filename.string()); it != m_images.end())
                    m_images.erase(it);
                break;
            }
            case AssetType::Sound:
            {
                std::lock_guard lock(m_soundsMutex);
                if (auto it = m_sounds.find(filename.string()); it != m_sounds.end())
                    m_sounds.erase(it);
                break;
            }
            case AssetType::Song:
            {
                std::lock_guard lock(m_songsMutex);
                if (auto it = m_songs.find(filename.string()); it != m_songs.end())
                    m_songs.erase(it);
                break;
            }
            case AssetType::Font:
            {
                std::lock_guard lock(m_fontsMutex);
                if (auto it = m_fonts.find(filename.string()); it != m_fonts.end())
                    m_fonts.erase(it);
                break;
            }
        }
    }

    void AssetManager::unload_list(const AssetList& assets)
    {
        for (const auto& asset : assets)
            unload(asset);
    }

    bool AssetManager::loading(void) const
    {
        return m_loading;
    }

    void AssetManager::load_worker(void)
    {
        DRK_PROFILE_FUNCTION();

        while (m_working)
        {
            bool ready;
            {
                DRK_PROFILE_SCOPE("Check queue");
                ready = !m_loadQueue.empty();
            }
            if (ready)
            {
                m_loading = true;
                AssetInfo asset = m_loadQueue.pop();
                load_impl(asset);
            }
            else
            {
                m_loading = false;
                std::this_thread::sleep_for(Time::Milli<int>(10));
            }
        }
    }

    void AssetManager::load_impl(const AssetInfo& asset)
    {
        DRK_PROFILE_FUNCTION();

        const auto& [type, filename] = asset;
        switch (type)
        {
            case AssetType::Font  : load_font(filename);  break;
            case AssetType::Image : load_image(filename); break;
            case AssetType::Song  : load_song(filename);  break;
            case AssetType::Sound : load_sound(filename); break;
        }
    }

    void AssetManager::load_image(const std::filesystem::path& filename)
    {
        std::lock_guard lock(m_imagesMutex);
    }

    void AssetManager::load_sound(const std::filesystem::path& filename)
    {
        std::lock_guard lock(m_soundsMutex);
        m_sounds[filename.string()] = Audio::load_file(sound_asset_path(filename));
    }

    void AssetManager::load_song(const std::filesystem::path& filename)
    {
        std::lock_guard lock(m_songsMutex);
        m_songs[filename.string()] = Audio::load_file(music_asset_path(filename));
    }

    void AssetManager::load_font(const std::filesystem::path& filename)
    {
        std::lock_guard lock(m_fontsMutex);
    }

    //////////////////////////////
    //       AssetLoader        //
    //////////////////////////////

    AssetLoader::AssetLoader(AssetManager& assetManager, const AssetList& assets)
      : m_assetManager(assetManager),
        m_assets(assets)
    {
        m_assetManager.load_list(m_assets);
    }

    AssetLoader::~AssetLoader(void)
    {
        m_assetManager.unload_list(m_assets);
    }
}
