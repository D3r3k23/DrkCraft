#ifndef DRK_ASSET_MANAGER_HPP
#define DRK_ASSET_MANAGER_HPP

#include "Core/Base.hpp"

#include <filesystem>

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

    class AssetManager
    {
    public:
        AssetManager(void);
        ~AssetManager(void);

    private:
    };
}

#endif // DRK_ASSET_MANAGER_HPP
