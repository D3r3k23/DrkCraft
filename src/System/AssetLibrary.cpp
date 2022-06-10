#include "AssetLibrary.hpp"

#include "Audio/Audio.hpp"
#include "Util/Image.hpp"
#include "Util/Obj.hpp"
#include "Util/Time.hpp"
#include "Core/Debug/Profiler.hpp"

#include <magic_enum.hpp>

#include <algorithm>
#include <utility>

namespace DrkCraft
{
    static const fs::path ASSET_DIR{"assets"};
    static const fs::path AUDIO_ASSET_DIR   = ASSET_DIR       / "audio";
    static const fs::path MUSIC_ASSET_DIR   = AUDIO_ASSET_DIR / "music";
    static const fs::path SOUND_ASSET_DIR   = AUDIO_ASSET_DIR / "sounds";
    static const fs::path IMAGE_ASSET_DIR   = ASSET_DIR       / "images";
    static const fs::path ICON_ASSET_DIR    = IMAGE_ASSET_DIR / "icons";
    static const fs::path TEXTURE_ASSET_DIR = IMAGE_ASSET_DIR / "textures";
    static const fs::path FONT_ASSET_DIR    = ASSET_DIR       / "fonts";
    static const fs::path MESH_ASSET_DIR    = ASSET_DIR       / "meshes";
    static const fs::path SHADER_ASSET_DIR  = ASSET_DIR       / "shaders";

    fs::path music_asset_path(const fs::path& filename)   { return MUSIC_ASSET_DIR   / filename; }
    fs::path sound_asset_path(const fs::path& filename)   { return SOUND_ASSET_DIR   / filename; }
    fs::path image_asset_path(const fs::path& filename)   { return IMAGE_ASSET_DIR   / filename; }
    fs::path icon_asset_path(const fs::path& filename)    { return ICON_ASSET_DIR    / filename; }
    fs::path texture_asset_path(const fs::path& filename) { return TEXTURE_ASSET_DIR / filename; }
    fs::path font_asset_path(const fs::path& filename)    { return FONT_ASSET_DIR    / filename; }
    fs::path mesh_asset_path(const fs::path& filename)    { return MESH_ASSET_DIR    / filename; }
    fs::path shader_asset_path(const fs::path& filename)  { return SHADER_ASSET_DIR  / filename; }

    ////////////////////////////////
    //       AssetLoadQueue       //
    ////////////////////////////////

    void AssetLibrary::AssetLoadQueue::push(const AssetInfo& asset)
    {
        DRK_PROFILE_FUNCTION();
        std::lock_guard lock(m_queueMutex);

        m_queue.push(asset);
    }

    void AssetLibrary::AssetLoadQueue::push(const AssetList& assets)
    {
        DRK_PROFILE_FUNCTION();
        std::lock_guard lock(m_queueMutex);

        for (const auto asset : assets)
            m_queue.push(asset);
    }

    AssetInfo AssetLibrary::AssetLoadQueue::pop(void)
    {
        DRK_PROFILE_FUNCTION();
        std::lock_guard lock(m_queueMutex);

        if (m_queue.empty())
            return {};
        else
        {
            DRK_PROFILE_SCOPE("Pop");
            const auto front = m_queue.front();
            m_queue.pop();
            return front;
        }
    }

    bool AssetLibrary::AssetLoadQueue::empty(void)
    {
        DRK_PROFILE_FUNCTION();
        std::lock_guard lock(m_queueMutex);

        return m_queue.empty();
    }

    //////////////////////////////
    //       AssetLibrary       //
    //////////////////////////////

    static constexpr auto LOAD_WORKER_SLEEP_TIME = Time::Milli<int>(10);

    AssetLibrary::AssetLibrary(void)
      : m_loading(false)
    {
        DRK_PROFILE_FUNCTION();

        m_loadThread = Thread<StopToken>("asset_load_thread", DRK_BIND_FN(load_worker));
    }

    AssetLibrary::~AssetLibrary(void)
    {

    }

    void AssetLibrary::load_worker(StopToken st)
    {
        DRK_PROFILE_FUNCTION();

        while (!st.stop_requested())
        {
            bool ready = !m_loadQueue.empty();
            if (ready)
            {
                DRK_PROFILE_EVENT_LOCAL("Load asset");
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

    void AssetLibrary::stop_loading(void)
    {
        m_loadThread.stop();
    }

    void AssetLibrary::unload_all(void)
    {
        {
            std::lock_guard lock(m_texturesMutex);
            m_textures.clear();
        }{
            std::lock_guard lock(m_audioSourcesMutex);
            m_audioSources.clear();
        }{
            std::lock_guard lock(m_meshesMutex);
            m_meshes.clear();
        }
    }

    void AssetLibrary::load(const AssetInfo& asset)
    {
        m_loadQueue.push(asset);
    }

    void AssetLibrary::load_list(const AssetList& assets)
    {
        m_loadQueue.push(assets);
    }

    void AssetLibrary::load_impl(const AssetInfo& asset)
    {
        DRK_PROFILE_FUNCTION();

        const auto& [type, filename] = asset;
        const auto assetTypeName  = magic_enum::enum_name(type);
        const auto assetName   = filename.generic_string();
        {
            std::lock_guard lock(m_recentlyLoadedAssetMutex);
            m_recentlyLoadedAsset = assetName;
        }
        DRK_LOG_CORE_INFO("Loading {} asset \"{}\"", assetTypeName, assetName);
        switch (type)
        {
            case AssetType::Texture : load_texture(texture_asset_path(filename));  break;
            case AssetType::Song   : load_audio_source(music_asset_path(filename)); break;
            case AssetType::Sound : load_audio_source(sound_asset_path(filename)); break;
            case AssetType::Mesh : load_mesh(mesh_asset_path(filename));          break;
            default:
                DRK_ASSERT_DEBUG(false, "Invalid AssetType");
                return;
        }
    }

    void AssetLibrary::unload(const AssetInfo& asset)
    {
        DRK_PROFILE_FUNCTION();

        const auto& [type, filename] = asset;
        switch (type)
        {
            case AssetType::Texture : unload_texture(texture_asset_path(filename));  break;
            case AssetType::Song   : unload_audio_source(music_asset_path(filename)); break;
            case AssetType::Sound : unload_audio_source(sound_asset_path(filename)); break;
            case AssetType::Mesh : unload_mesh(mesh_asset_path(filename));          break;
            default:
                DRK_ASSERT_DEBUG(false, "Invalid AssetType");
                return;
        }
        DRK_LOG_CORE_INFO("Asset \"{}\" unloaded", filename.generic_string());
    }

    void AssetLibrary::unload_list(const AssetList& assets)
    {
        for (const auto& asset : assets)
            unload(asset);
    }

    bool AssetLibrary::texture_loaded(const fs::path& filename) const
    {
        std::lock_guard lock(m_texturesMutex);
        return m_textures.contains(filename.string());
    }

    Ref<Texture> AssetLibrary::get_texture(const fs::path& filename) const
    {
        std::lock_guard lock(m_texturesMutex);
        if (m_textures.contains(filename.string()))
            return m_textures.at(filename.string());
        else
            return {};
    }

    bool AssetLibrary::song_loaded(const fs::path& filename) const
    {
        std::lock_guard lock(m_audioSourcesMutex);
        return m_audioSources.contains(filename.string());
    }

    bool AssetLibrary::sound_loaded(const fs::path& filename) const
    {
        std::lock_guard lock(m_audioSourcesMutex);
        return m_audioSources.contains(filename.string());
    }

    Ref<AudioSource> AssetLibrary::get_song(const fs::path& filename) const
    {
        return get_sound(filename);
    }

    Ref<AudioSource> AssetLibrary::get_sound(const fs::path& filename) const
    {
        std::lock_guard lock(m_audioSourcesMutex);
        if (m_audioSources.contains(filename.string()))
            return m_audioSources.at(filename.string());
        else
            return {};
    }

    bool AssetLibrary::mesh_loaded(const fs::path& filename) const
    {
        std::lock_guard lock(m_meshesMutex);
        return m_meshes.contains(filename.string());
    }

    Ref<Mesh> AssetLibrary::get_mesh(const fs::path& filename) const
    {
        std::lock_guard lock(m_meshesMutex);
        if (m_meshes.contains(filename.string()))
            return m_meshes.at(filename.string());
        else
            return {};
    }

    bool AssetLibrary::loading(void) const
    {
        return m_loading;
    }

    std::optional<std::string> AssetLibrary::currently_loading(void) const
    {
        if (loading())
        {
            std::lock_guard lock(m_recentlyLoadedAssetMutex);
            return m_recentlyLoadedAsset;
        }
        else
            return {};
    }

    void AssetLibrary::load_texture(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();

        if (const auto image = Image::load_file(filename); image)
            if (const auto texture = Texture::from_image(*image); texture)
            {
                std::lock_guard lock(m_texturesMutex);
                m_textures[filename.string()] = std::move(texture);
            }
    }

    void AssetLibrary::load_audio_source(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();

        if (const auto source = Audio::load_file(filename); source)
        {
            std::lock_guard lock(m_audioSourcesMutex);
            m_audioSources[filename.string()] = std::move(source);
        }
    }

    void AssetLibrary::load_mesh(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();

        if (const auto data = Obj::load_file(filename); data)
            if (const auto mesh = Mesh::create(*data); mesh)
            {
                std::lock_guard lock(m_meshesMutex);
                m_meshes[filename.string()] = std::move(mesh);
            }
    }

    void AssetLibrary::unload_texture(const fs::path& filename)
    {
        std::lock_guard lock(m_texturesMutex);
        if (auto it = m_textures.find(filename.string()); it != m_textures.end())
            m_textures.erase(it);
    }

    void AssetLibrary::unload_audio_source(const fs::path& filename)
    {
        std::lock_guard lock(m_audioSourcesMutex);
        if (auto it = m_audioSources.find(filename.string()); it != m_audioSources.end())
            m_audioSources.erase(it);
    }

    void AssetLibrary::unload_mesh(const fs::path& filename)
    {
        std::lock_guard lock(m_meshesMutex);
        if (auto it = m_meshes.find(filename.string()); it != m_meshes.end())
            m_meshes.erase(it);
    }

    //////////////////////////////
    //       AssetLoader        //
    //////////////////////////////

    AssetLoader::AssetLoader(AssetLibrary& library, const AssetList& assets)
      : m_library(library),
        m_assets(assets)
    {
        m_library.load_list(m_assets);
    }

    AssetLoader::~AssetLoader(void)
    {
        m_library.unload_list(m_assets);
    }
}
