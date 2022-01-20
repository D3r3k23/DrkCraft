#ifndef DRK_ASSET_MANAGER_HPP
#define DRK_ASSET_MANAGER_HPP

#include "Core/Base.hpp"
#include "Audio/AudioSource.hpp"
#include "System/Image.hpp"
#include "System/Font.hpp"

#include <string>
#include <filesystem>
#include <vector>
#include <queue>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <atomic>
#include <utility>

namespace DrkCraft
{
    std::filesystem::path font_asset_path(const std::filesystem::path& filename);
    std::filesystem::path icon_asset_path(const std::filesystem::path& filename);
    std::filesystem::path image_asset_path(const std::filesystem::path& filename);
    std::filesystem::path model_asset_path(const std::filesystem::path& filename);
    std::filesystem::path music_asset_path(const std::filesystem::path& filename);
    std::filesystem::path shader_asset_path(const std::filesystem::path& filename);
    std::filesystem::path sound_asset_path(const std::filesystem::path& filename);
    std::filesystem::path texture_asset_path(const std::filesystem::path& filename);

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

    using AssetInfo = std::pair<AssetType, std::filesystem::path>;
    using AssetList = std::vector<AssetInfo>;

    class AssetManager
    {
    private:
        class AssetLoadQueue
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

        bool sound_loaded(const std::filesystem::path& filename);
        bool song_loaded(const std::filesystem::path& filename);

        Ref<AudioSource> get_sound(const std::filesystem::path& filename);
        Ref<AudioSource> get_song(const std::filesystem::path& filename);

        void unload(const AssetInfo& asset);
        void unload_list(const AssetList& assets);

        bool loading(void) const;

    private:
        void load_worker(void);
        void load_impl(const AssetInfo& asset);

        void load_image(const std::filesystem::path& filename);

        void load_sound(const std::filesystem::path& filename);
        void load_song(const std::filesystem::path& filename);

        void load_font(const std::filesystem::path& filename);

    private:
        AssetLoadQueue m_loadQueue;
        std::thread m_loadThread;
        std::atomic<bool> m_working;
        std::atomic<bool> m_loading;

        std::unordered_map<std::string, Ref<Image>> m_images;
        std::unordered_map<std::string, Ref<AudioSource>> m_sounds;
        std::unordered_map<std::string, Ref<AudioSource>> m_songs;
        std::unordered_map<std::string, Font> m_fonts;

        std::mutex m_imagesMutex;
        std::mutex m_soundsMutex;
        std::mutex m_songsMutex;
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

#endif // DRK_ASSET_MANAGER_HPP
