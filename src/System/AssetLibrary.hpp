#ifndef DRK_SYSTEM_ASSET_LIBRARY_HPP
#define DRK_SYSTEM_ASSET_LIBRARY_HPP

#include "Core/Base.hpp"
#include "Audio/AudioSource.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Mesh.hpp"
#include "System/Thread.hpp"
#include "System/Mutex.hpp"

#include "lib/fs.hpp"
#include "lib/string.hpp"

#include <vector>
#include <queue>
#include <unordered_map>
#include <atomic>

namespace DrkCraft
{
    fs::path music_asset_path(const fs::path& filename);
    fs::path sound_asset_path(const fs::path& filename);
    fs::path image_asset_path(const fs::path& filename);
    fs::path icon_asset_path(const fs::path& filename);
    fs::path texture_asset_path(const fs::path& filename);
    fs::path font_asset_path(const fs::path& filename);
    fs::path mesh_asset_path(const fs::path& filename);
    fs::path shader_asset_path(const fs::path& filename);

    enum class AssetType
    {
        None = 0,
        Font,
        Icon,
        Image,
        Mesh,
        Shader,
        Song,
        Sound,
        Texture
    };

    struct AssetInfo
    {
        AssetType type;
        fs::path path;
    };

    using AssetList = std::vector<AssetInfo>;

    template <typename A>
    using AssetContainer = std::unordered_map<string, A>;

    class AssetLibrary
    {
    private:
        class AssetLoadQueue // Thread-safe
        {
        public:
            AssetLoadQueue(void) = default;

            void push(const AssetInfo& asset);
            void push(const AssetList& assets);
            AssetInfo pop(void);

            bool empty(void);

        private:
            std::queue<AssetInfo> m_queue;
            Mutex m_queueMutex{"asset_load_queue_mutex"};
        };

    public:
        AssetLibrary(void);
        ~AssetLibrary(void);

    private:
        void load_worker(StopToken st);

    public:
        void stop_loading(void);
        void unload_all(void);

        void load(const AssetInfo& asset);
        void load_list(const AssetList& assets);

        void load_impl(const AssetInfo& asset);

        void unload(const AssetInfo& asset);
        void unload_list(const AssetList& assets);

        bool texture_loaded(const fs::path& filename) const;
        Ref<Texture> get_texture(const fs::path& filename) const;

        bool song_loaded(const fs::path& filename) const;
        bool sound_loaded(const fs::path& filename) const;
        Ref<AudioSource> get_song(const fs::path& filename) const;
        Ref<AudioSource> get_sound(const fs::path& filename) const;

        bool mesh_loaded(const fs::path& filename) const;
        Ref<Mesh> get_mesh(const fs::path& filename) const;

        bool loading(void) const;
        Optional<string> currently_loading(void) const;

    private:
        void load_texture(const fs::path& filename);
        void load_audio_source(const fs::path& filename);
        void load_mesh(const fs::path& filename);
        void unload_texture(const fs::path& filename);
        void unload_audio_source(const fs::path& filename);
        void unload_mesh(const fs::path& filename);

    private:
        AssetLoadQueue m_loadQueue;
        Thread<StopToken> m_loadThread;

        std::atomic<bool> m_loading;
        string m_recentlyLoadedAsset;

        AssetContainer<Ref<Texture>> m_textures;
        AssetContainer<Ref<AudioSource>> m_audioSources;
        AssetContainer<Ref<Mesh>> m_meshes;

        mutable Mutex m_texturesMutex{"texture_asset_mutex"};
        mutable Mutex m_audioSourcesMutex{"audio_source_asset_mutex"};
        mutable Mutex m_meshesMutex{"mesh_asset_mutex"};

        mutable Mutex m_recentlyLoadedAssetMutex{"recently_loaded_asset_mutex"};
    };

    class AssetLoader
    {
    public:
        AssetLoader(AssetLibrary& library, const AssetList& assets);
        ~AssetLoader(void);

    private:
        AssetLibrary& m_library;
        AssetList m_assets;
    };
}

#endif // DRK_SYSTEM_ASSET_LIBRARY_HPP
