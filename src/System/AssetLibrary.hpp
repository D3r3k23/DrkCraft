#ifndef DRK_SYSTEM_ASSET_LIBRARY_HPP
#define DRK_SYSTEM_ASSET_LIBRARY_HPP

#include "Core/Base.hpp"
#include "Audio/AudioSource.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Mesh.hpp"

#include "System/Image.hpp"

#include "lib/fs.hpp"

#include <string>
#include <optional>
#include <vector>
#include <queue>
#include <unordered_map>
#include <thread>
#include <mutex>
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
    using AssetContainer = std::unordered_map<std::string, A>;

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
            std::mutex m_queueMutex;
        };

    public:
        AssetLibrary(void);
        ~AssetLibrary(void);

    private:
        void load_worker(std::stop_token st);

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
        std::optional<std::string> currently_loading(void) const;

    private:
        void load_texture(const fs::path& filename);
        void load_audio_source(const fs::path& filename);
        void load_mesh(const fs::path& filename);
        void unload_texture(const fs::path& filename);
        void unload_audio_source(const fs::path& filename);
        void unload_mesh(const fs::path& filename);

    private:
        AssetLoadQueue m_loadQueue;
        std::jthread m_loadThread;

        std::atomic<bool> m_loading;
        std::string m_recentlyLoadedAsset;

        AssetContainer<Ref<Texture>> m_textures;
        AssetContainer<Ref<AudioSource>> m_audioSources;
        AssetContainer<Ref<Mesh>> m_meshes;

        mutable std::mutex m_texturesMutex;
        mutable std::mutex m_audioSourcesMutex;
        mutable std::mutex m_meshesMutex;

        mutable std::mutex m_recentlyLoadedAssetMutex;
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