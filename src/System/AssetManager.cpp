#include "AssetManager.hpp"

#include "Audio/Audio.hpp"
#include <Core/Time.hpp>
#include "Core/Profiler.hpp"

#include <nameof.hpp>

#include <algorithm>

namespace DrkCraft
{
    const auto LOAD_WORKER_SLEEP_TIME = Time::Milli<int>(10);

    namespace fs = std::filesystem;

    static const fs::path ASSET_DIR = "assets";
    static const fs::path FONT_ASSET_DIR    = ASSET_DIR / "fonts";
    static const fs::path ICON_ASSET_DIR    = ASSET_DIR / "icons";
    static const fs::path IMAGE_ASSET_DIR   = ASSET_DIR / "images";
    static const fs::path MODEL_ASSET_DIR   = ASSET_DIR / "models";
    static const fs::path MUSIC_ASSET_DIR   = ASSET_DIR / "music";
    static const fs::path SHADER_ASSET_DIR  = ASSET_DIR / "shaders";
    static const fs::path SOUND_ASSET_DIR   = ASSET_DIR / "sounds";
    static const fs::path TEXTURE_ASSET_DIR = ASSET_DIR / "textures";

    fs::path font_asset_path(const fs::path& filename)    { return FONT_ASSET_DIR    / filename; }
    fs::path icon_asset_path(const fs::path& filename)    { return ICON_ASSET_DIR    / filename; }
    fs::path image_asset_path(const fs::path& filename)   { return IMAGE_ASSET_DIR   / filename; }
    fs::path model_asset_path(const fs::path& filename)   { return MODEL_ASSET_DIR   / filename; }
    fs::path music_asset_path(const fs::path& filename)   { return MUSIC_ASSET_DIR   / filename; }
    fs::path shader_asset_path(const fs::path& filename)  { return SHADER_ASSET_DIR  / filename; }
    fs::path sound_asset_path(const fs::path& filename)   { return SOUND_ASSET_DIR   / filename; }
    fs::path texture_asset_path(const fs::path& filename) { return TEXTURE_ASSET_DIR / filename; }

    ////////////////////////////////
    //       AssetLoadQueue       //
    ////////////////////////////////

    void AssetManager::AssetLoadQueue::push(const AssetInfo& asset)
    {
        DRK_PROFILE_FUNCTION();
        std::lock_guard lock(m_queueMutex);

        m_queue.push(asset);
    }

    void AssetManager::AssetLoadQueue::push(const AssetList& assets)
    {
        DRK_PROFILE_FUNCTION();
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

        DRK_PROFILE_SCOPE("Pop");
        const auto front = m_queue.front();
        m_queue.pop();
        return front;
    }

    bool AssetManager::AssetLoadQueue::empty(void)
    {
        DRK_PROFILE_FUNCTION();
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

        DRK_PROFILE_THREAD_CREATE("Asset load thread");
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
        {
            std::lock_guard lock(m_imagesMutex);
            m_images.clear();
        }{
            std::lock_guard lock(m_soundsMutex);
            m_sounds.clear();
        }{
            std::lock_guard lock(m_songsMutex);
            m_songs.clear();
        }{
            std::lock_guard lock(m_fontsMutex);
            m_fonts.clear();
        }
    }

    void AssetManager::load(const AssetInfo& asset)
    {
        m_loadQueue.push(asset);
    }

    void AssetManager::load_list(const AssetList& assets)
    {
        m_loadQueue.push(assets);
    }

    bool AssetManager::sound_loaded(const fs::path& filename)
    {
        std::lock_guard lock(m_soundsMutex);
        return m_sounds.contains(filename.string());
    }

    bool AssetManager::song_loaded(const fs::path& filename)
    {
        std::lock_guard lock(m_songsMutex);
        return m_songs.contains(filename.string());
    }

    Ref<AudioSource> AssetManager::get_sound(const fs::path& filename)
    {
        std::lock_guard lock(m_soundsMutex);
        DRK_ASSERT_DEBUG(m_sounds.contains(filename.string()), "Sound not loaded");
        return m_sounds[filename.string()];
    }

    Ref<AudioSource> AssetManager::get_song(const fs::path& filename)
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
        DRK_PROFILE_THREAD_START("Asset load thread");
        DRK_PROFILE_FUNCTION();

        while (m_working)
        {
            bool ready = !m_loadQueue.empty();
            if (ready)
            {
                DRK_PROFILE_EVENT("Load asset");
                m_loading = true;
                AssetInfo asset = m_loadQueue.pop();
                load_impl(asset);
            }
            else
            {
                m_loading = false;
                DRK_PROFILE_SCOPE("Sleep");
                std::this_thread::sleep_for(LOAD_WORKER_SLEEP_TIME);
            }
        }
    }

    void AssetManager::load_impl(const AssetInfo& asset)
    {
        DRK_PROFILE_FUNCTION();

        const auto& [type, filename] = asset;
        const auto assetTypeName = NAMEOF_ENUM(type);
        const auto assetName = filename.generic_string();

        DRK_LOG_CORE_INFO("Loading {} asset \"{}\"", assetTypeName, assetName);
        switch (type)
        {
            case AssetType::Image : load_image(filename); break;
            case AssetType::Song  : load_song(filename);  break;
            case AssetType::Sound : load_sound(filename); break;
            case AssetType::Font  : load_font(filename);  break;
        }
        DRK_LOG_CORE_INFO("{} asset \"{}\" loaded", assetTypeName, assetName);
    }

    void AssetManager::load_image(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();
        std::lock_guard lock(m_imagesMutex);
    }

    void AssetManager::load_sound(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();
        std::lock_guard lock(m_soundsMutex);
        m_sounds[filename.string()] = Audio::load_file(sound_asset_path(filename));
    }

    void AssetManager::load_song(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();
        std::lock_guard lock(m_songsMutex);
        m_songs[filename.string()] = Audio::load_file(music_asset_path(filename));
    }

    void AssetManager::load_font(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();
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
