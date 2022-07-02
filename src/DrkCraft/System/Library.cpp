#include "Library.hpp"

#include "Audio/Audio.hpp"
#include "Disk/Image.hpp"
#include "Disk/Obj.hpp"
#include "Util/Time.hpp"
#include "System/Lock.hpp"
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
    //       AssetLoadToken       //
    ////////////////////////////////

    AssetLoadToken::AssetLoadToken(AssetLoadStatus status=AssetLoadStatus::None)
      : m_status(status)
    { }

    void AssetLoadToken::set(AssetLoadStatus status)
    {
        m_status = status;
    }

    AssetLoadStatus AssetLoadToken::status(void)
    {
        return m_status;
    }

    bool AssetLoadToken::complete(void) const
    {
        return m_status == AssetLoadStatus::Complete;
    }

    ////////////////////////////////
    //       AssetLoadQueue       //
    ////////////////////////////////

    void Library::AssetLoadQueue::add_callback(AssetLoadedCallbackFn fn)
    {
        DRK_PROFILE_FUNCTION();
        Lock<> lock(m_queueMutex);

        m_queue.push(std::move(fn));
    }

    void Library::AssetLoadQueue::push(const AssetInfo& asset)
    {
        DRK_PROFILE_FUNCTION();
        Lock<> lock(m_queueMutex);

        m_queue.push(asset);
    }

    void Library::AssetLoadQueue::push(AssetList assets)
    {
        DRK_PROFILE_FUNCTION();
        Lock<> lock(m_queueMutex);

        for (const auto& asset : assets)
            m_queue.push(asset);
    }

    void Library::AssetLoadQueue::push(ItemList items)
    {
        DRK_PROFILE_FUNCTION();
        Lock<> lock(m_queueMutex);

        for (const auto& item : items)
            m_queue.push(item);
    }

    AssetInfo Library::AssetLoadQueue::pop(void)
    {
        DRK_PROFILE_FUNCTION();
        Lock<> lock(m_queueMutex);

        if (m_queue.empty())
            return {};
        else
        {
            DRK_PROFILE_SCOPE("Pop");
            const auto front = m_queue.front();
            m_queue.pop();
            if (!m_queue.empty())
                if (const auto next = m_queue.front().get_if<AssetLoadedCallbackFn>(); next)
                {
                    DRK_LOG_CORE_TRACE("Calling AssetLoadedCallbackFn");
                    (*next)();
                    m_queue.pop();
                }
            return front.get<AssetInfo>();
        }
    }

    bool Library::AssetLoadQueue::empty(void)
    {
        DRK_PROFILE_FUNCTION();
        Lock<> lock(m_queueMutex);

        return m_queue.empty();
    }

    //////////////////////////////
    //       Library       //
    //////////////////////////////

    static constexpr auto LOAD_WORKER_SLEEP_TIME = Time::Milli<int>(10);

    Library::Library(void)
      : m_loading(false)
    {
        DRK_PROFILE_FUNCTION();

        m_loadThread = Thread<StopToken>("asset_load_thread", DRK_BIND_FN(load_worker));
    }

    Library::~Library(void)
    {

    }

    void Library::load_worker(StopToken st)
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
                Thread<>::This::sleep_for(LOAD_WORKER_SLEEP_TIME);
            }
        }
    }

    void Library::stop_loading(void)
    {
        m_loadThread.stop();
    }

    void Library::unload_all(void)
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

    void Library::load(const AssetInfo& asset)
    {
        m_loadQueue.push(asset);
    }

    void Library::load_list(AssetList assets)
    {
        m_loadQueue.push(assets);
    }

    void Library::load_list(AssetList assets, AssetLoadedCallbackFn fn)
    {
        m_loadQueue.push(assets);
        m_loadQueue.add_callback(std::move(fn));
    }

    Ptr<AssetLoadToken> Library::load_list_and_get_token(AssetList assets)
    {
        auto token = make_ptr<AssetLoadToken>(AssetLoadStatus::Loading);
        load_list(assets, [token=token.get()]
        {
            token->set(AssetLoadStatus::Complete);
        });
        return std::move(token);
    }

    void Library::load_impl(const AssetInfo& asset)
    {
        DRK_PROFILE_FUNCTION();

        const auto& [type, filename] = asset;
        const auto assetTypeName  = magic_enum::enum_name(type);
        const auto assetName   = filename.generic_string();
        {
            Lock<> lock(m_recentlyLoadedAssetMutex);
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
                DRK_ASSERT_DEBUG_FALSE("Invalid AssetType");
                return;
        }
    }

    void Library::unload(const AssetInfo& asset)
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
                DRK_ASSERT_DEBUG_FALSE("Invalid AssetType");
                return;
        }
        DRK_LOG_CORE_INFO("Asset \"{}\" unloaded", filename.generic_string());
    }

    void Library::unload_list(const AssetList& assets)
    {
        for (const auto& asset : assets)
            unload(asset);
    }

    bool Library::texture_loaded(const fs::path& filename) const
    {
        Lock<> lock(m_texturesMutex);
        return m_textures.contains(filename.string());
    }

    Ref<Texture> Library::get_texture(const fs::path& filename) const
    {
        Lock<> lock(m_texturesMutex);
        if (m_textures.contains(filename.string()))
            return m_textures.at(filename.string());
        else
            return {};
    }

    bool Library::song_loaded(const fs::path& filename) const
    {
        Lock<> lock(m_audioSourcesMutex);
        return m_audioSources.contains(filename.string());
    }

    bool Library::sound_loaded(const fs::path& filename) const
    {
        Lock<> lock(m_audioSourcesMutex);
        return m_audioSources.contains(filename.string());
    }

    Ref<AudioSource> Library::get_song(const fs::path& filename) const
    {
        return get_sound(filename);
    }

    Ref<AudioSource> Library::get_sound(const fs::path& filename) const
    {
        Lock<> lock(m_audioSourcesMutex);
        if (m_audioSources.contains(filename.string()))
            return m_audioSources.at(filename.string());
        else
            return {};
    }

    bool Library::mesh_loaded(const fs::path& filename) const
    {
        Lock<> lock(m_meshesMutex);
        return m_meshes.contains(filename.string());
    }

    Ref<Mesh> Library::get_mesh(const fs::path& filename) const
    {
        Lock<> lock(m_meshesMutex);
        if (m_meshes.contains(filename.string()))
            return m_meshes.at(filename.string());
        else
            return {};
    }

    bool Library::loading(void) const
    {
        return m_loading;
    }

    Optional<string> Library::currently_loading(void) const
    {
        if (loading())
        {
            Lock<> lock(m_recentlyLoadedAssetMutex);
            return m_recentlyLoadedAsset;
        }
        else
            return {};
    }

    void Library::load_texture(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();

        if (const auto image = Image::load_file(filename); image)
            if (const auto texture = Texture::from_image(*image); texture)
            {
                Lock<> lock(m_texturesMutex);
                m_textures[filename.string()] = std::move(texture);
            }
    }

    void Library::load_audio_source(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();

        if (const auto source = Audio::load_file(filename); source)
        {
            Lock<> lock(m_audioSourcesMutex);
            m_audioSources[filename.string()] = std::move(source);
        }
    }

    void Library::load_mesh(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();

        if (const auto data = Obj::load_file(filename); data)
            if (const auto mesh = Mesh::create(*data); mesh)
            {
                Lock<> lock(m_meshesMutex);
                m_meshes[filename.string()] = std::move(mesh);
            }
    }

    void Library::unload_texture(const fs::path& filename)
    {
        Lock<> lock(m_texturesMutex);
        if (auto it = m_textures.find(filename.string()); it != m_textures.end())
            m_textures.erase(it);
    }

    void Library::unload_audio_source(const fs::path& filename)
    {
        Lock<> lock(m_audioSourcesMutex);
        if (auto it = m_audioSources.find(filename.string()); it != m_audioSources.end())
            m_audioSources.erase(it);
    }

    void Library::unload_mesh(const fs::path& filename)
    {
        Lock<> lock(m_meshesMutex);
        if (auto it = m_meshes.find(filename.string()); it != m_meshes.end())
            m_meshes.erase(it);
    }

    //////////////////////////////
    //       AssetLoader        //
    //////////////////////////////

    AssetLoader::AssetLoader(Library& library, const AssetList& assets)
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
