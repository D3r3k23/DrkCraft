#ifndef DRK_SYSTEM_ASSET_MANAGER_HPP
#define DRK_SYSTEM_ASSET_MANAGER_HPP

#include "Core/Base.hpp"
#include "System/Audio/AudioSource.hpp"
#include "System/Image.hpp"
#include "System/Font.hpp"

#include "lib/fs.hpp"

#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <atomic>
#include <utility>

namespace DrkCraft
{
    fs::path music_asset_path(const fs::path& filename);
    fs::path sound_asset_path(const fs::path& filename);
    fs::path image_asset_path(const fs::path& filename);
    fs::path icon_asset_path(const fs::path& filename);
    fs::path texture_asset_path(const fs::path& filename);
    fs::path font_asset_path(const fs::path& filename);
    fs::path model_asset_path(const fs::path& filename);
    fs::path shader_asset_path(const fs::path& filename);

    enum class AssetType
    {
        None = 0,
        Font,
        Icon,
        Image,
        Model,
        Song,
        Shader,
        Sound,
        Texture
    };

    struct AssetInfo
    {
        AssetType type;
        fs::path path;
    };

    using AssetList = std::vector<AssetInfo>;

    class AssetManager
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
        AssetManager(void);
        ~AssetManager(void);

        void stop_loading(void);
        void unload_all(void);

        void load(const AssetInfo& asset);
        void load_list(const AssetList& assets);

        bool sound_loaded(const fs::path& filename);
        bool song_loaded(const fs::path& filename);

        Ref<AudioSource> get_sound(const fs::path& filename);
        Ref<AudioSource> get_song(const fs::path& filename);

        void unload(const AssetInfo& asset);
        void unload_list(const AssetList& assets);

        bool loading(void) const;
        std::string currently_loading(void) const;

    private:
        void load_worker(std::stop_token st);
        void load_impl(const AssetInfo& asset);

        void load_image(const fs::path& filename);

        void load_sound(const fs::path& filename);
        void load_song(const fs::path& filename);

        void load_font(const fs::path& filename);

    private:
        AssetLoadQueue m_loadQueue;
        std::jthread m_loadThread;
        std::atomic<bool> m_loading;
        std::atomic<std::string> m_recentlyLoadedAsset;

        std::unordered_map<std::string, Ref<Image>> m_images;
        std::unordered_map<std::string, Ref<AudioSource>> m_audioSources;
        std::unordered_map<std::string, Ref<Font>> m_fonts;

        std::mutex m_imagesMutex;
        std::mutex m_audioSourcesMutex;
        std::mutex m_fontsMutex;
    };

    class AssetLoader
    {
    public:
        AssetLoader(AssetManager& assetManager, const AssetList& assets);
        ~AssetLoader(void);

    private:
        AssetManager& m_assetManager;
        AssetList m_assets;
    };
}

#endif // DRK_SYSTEM_ASSET_MANAGER_HPP
