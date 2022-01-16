#ifndef DRK_ASSET_MANAGER_HPP
#define DRK_ASSET_MANAGER_HPP

#include "Core/Base.hpp"
#include "System/Image.hpp"
#include "Audio/AudioSource.hpp"
#include "System/Font.hpp"

#include <string>
#include <filesystem>
#include <thread>
#include <vector>
#include <unordered_map>
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

    using AssetLoadList = std::vector<std::pair<AssetType, std::filesystem::path>>;

    class AssetManager
    {
    public:
        AssetManager(void);
        ~AssetManager(void);

        void destroy(void);

        void load_list(const AssetLoadList& assets);

        void load_image(const std::filesystem::path& filename);

        Ref<AudioSource> load_sound(const std::filesystem::path& filename);
        Ref<AudioSource> load_song(const std::filesystem::path& filename);

        Ref<AudioSource> get_sound(const std::filesystem::path& filename);
        Ref<AudioSource> get_song(const std::filesystem::path& filename);

        void load_font(const std::filesystem::path& filename);
        void load_model(const std::filesystem::path& filename);

        bool busy(void) const;

    private:
        void load_list_impl(AssetLoadList assets);

    private:
        std::unordered_map<std::string, Ref<Image>> m_images;
        std::unordered_map<std::string, Ref<AudioSource>> m_sounds;
        std::unordered_map<std::string, Ref<AudioSource>> m_songs;
        std::unordered_map<std::string, Font> m_fonts;

        bool m_loading;
        std::thread m_loadThread;
    };
}

#endif // DRK_ASSET_MANAGER_HPP
