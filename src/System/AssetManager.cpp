#include "AssetManager.hpp"

#include "Core/Profiler.hpp"

#include "Audio/Audio.hpp"

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

    AssetManager::AssetManager(void)
      : m_loading(false)
    { }

    AssetManager::~AssetManager(void)
    {
        m_loadThread.join();
        destroy();
    }

    void AssetManager::destroy(void)
    {
        m_images.clear();
        m_sounds.clear();
        m_songs.clear();
        m_fonts.clear();
    }

    void AssetManager::load_list(const AssetLoadList& assets)
    {
        DRK_PROFILE_FUNCTION();

        if (busy())
        {
            DRK_LOG_CORE_WARN("AssetManager is already loading");
            return;
        }
        m_loading = true;
        m_loadThread = std::thread(DRK_BIND_FN(load_list_impl), assets);
    }

    void AssetManager::load_image(const std::filesystem::path& filename)
    {

    }

    Ref<AudioSource> AssetManager::load_sound(const std::filesystem::path& filename)
    {
        auto sound = Audio::load_file(sound_asset_path(filename));
        m_sounds[filename.string()] = sound;
        return sound;
    }

    Ref<AudioSource> AssetManager::load_song(const std::filesystem::path& filename)
    {
        auto song = Audio::load_file(music_asset_path(filename));
        m_songs[filename.string()] = song;
        return song;
    }

    Ref<AudioSource> AssetManager::get_sound(const std::filesystem::path& filename)
    {
        DRK_ASSERT_DEBUG(m_sounds.contains(filename.string()), "Sound not loaded");
        return m_sounds[filename.string()];
    }

    Ref<AudioSource> AssetManager::get_song(const std::filesystem::path& filename)
    {
        DRK_ASSERT_DEBUG(m_songs.contains(filename.string()), "Song not loaded");
        return m_songs[filename.string()];
    }

    void AssetManager::load_font(const std::filesystem::path& filename)
    {

    }

    void AssetManager::load_model(const std::filesystem::path& filename)
    {

    }

    bool AssetManager::busy(void) const
    {
        return m_loading;
    }

    void AssetManager::load_list_impl(AssetLoadList assets)
    {
        DRK_PROFILE_FUNCTION();

        for (const auto& asset : assets)
        {
            const auto& [type, filename] = asset;
            switch (type)
            {
                case AssetType::Font  : load_font(filename);  break;
                case AssetType::Image : load_image(filename); break;
                case AssetType::Model : load_model(filename); break;
                case AssetType::Song  : load_song(filename);  break;
                case AssetType::Sound : load_sound(filename); break;
            }
        }
        m_loading = false;
        DRK_LOG_CORE_INFO("AssetManager finished loading");
    }
}
