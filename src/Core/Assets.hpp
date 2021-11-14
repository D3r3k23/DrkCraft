#ifndef DRK_ASSETS_HPP
#define DRK_ASSETS_HPP

#include "Base.hpp"

#include <string_view>

#define DRK_FONT_ASSET_DIR    "assets/fonts"
#define DRK_ICON_ASSET_DIR    "assets/icons"
#define DRK_IMAGE_ASSET_DIR   "assets/images"
#define DRK_MODEL_ASSET_DIR   "assets/models"
#define DRK_MUSIC_ASSET_DIR   "assets/music"
#define DRK_SHADER_ASSET_DIR  "assets/shaders"
#define DRK_SOUND_ASSET_DIR   "assets/sounds"
#define DRK_TEXTURE_ASSET_DIR "assets/textures"

#define DRK_CONCATENATE_PATH(path1, path2) path1"/"path2

#define DRK_FONT_ASSET_PATH(name)    DRK_CONCATENATE_PATH(DRK_FONT_ASSET_DIR, name)
#define DRK_ICON_ASSET_PATH(name)    DRK_CONCATENATE_PATH(DRK_ICON_ASSET_DIR, name)
#define DRK_IMAGE_ASSET_PATH(name)   DRK_CONCATENATE_PATH(DRK_IMAGE_ASSET_DIR, name)
#define DRK_MODEL_ASSET_PATH(name)   DRK_CONCATENATE_PATH(DRK_MODEL_ASSET_DIR, name)
#define DRK_MUSIC_ASSET_PATH(name)   DRK_CONCATENATE_PATH(DRK_MUSIC_ASSET_DIR, name)
#define DRK_SHADER_ASSET_PATH(name)  DRK_CONCATENATE_PATH(DRK_SHADER_ASSET_DIR, name)
#define DRK_SOUND_ASSET_PATH(name)   DRK_CONCATENATE_PATH(DRK_SOUND_ASSET_DIR, name)
#define DRK_TEXTURE_ASSET_PATH(name) DRK_CONCATENATE_PATH(DRK_TEXTURE_ASSET_DIR, name)

namespace DrkCraft
{
    class Assets
    {
    public:

    private:
    };
}

#endif // DRK_ASSETS_HPP
