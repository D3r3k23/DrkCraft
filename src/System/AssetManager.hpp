#ifndef DRK_ASSET_MANAGER_HPP
#define DRK_ASSET_MANAGER_HPP

#include "Core/Base.hpp"

#include <string>
#include <string_view>

namespace DrkCraft
{
    std::string font_asset_path(std::string_view name);
    std::string icon_asset_path(std::string_view name);
    std::string image_asset_path(std::string_view name);
    std::string model_asset_path(std::string_view name);
    std::string music_asset_path(std::string_view name);
    std::string shader_asset_path(std::string_view name);
    std::string sound_asset_path(std::string_view name);
    std::string texture_asset_path(std::string_view name);

    class AssetManager
    {
    public:

    private:
    };
}

#endif // DRK_ASSET_MANAGER_HPP
