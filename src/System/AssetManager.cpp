#include "AssetManager.hpp"

#include "Audio/Audio.hpp"

namespace DrkCraft
{
    using std::filesystem::path;

    static const path ASSET_DIR = "assets";
    static const path FONT_ASSET_DIR    = ASSET_DIR / "fonts";
    static const path ICON_ASSET_DIR    = ASSET_DIR / "icons";
    static const path IMAGE_ASSET_DIR   = ASSET_DIR / "images";
    static const path MODEL_ASSET_DIR   = ASSET_DIR / "models";
    static const path MUSIC_ASSET_DIR   = ASSET_DIR / "music";
    static const path SHADER_ASSET_DIR  = ASSET_DIR / "shaders";
    static const path SOUND_ASSET_DIR   = ASSET_DIR / "sounds";
    static const path TEXTURE_ASSET_DIR = ASSET_DIR / "textures";

    path font_asset_path(const path& filename)    { return FONT_ASSET_DIR    / filename; }
    path icon_asset_path(const path& filename)    { return ICON_ASSET_DIR    / filename; }
    path image_asset_path(const path& filename)   { return IMAGE_ASSET_DIR   / filename; }
    path model_asset_path(const path& filename)   { return MODEL_ASSET_DIR   / filename; }
    path music_asset_path(const path& filename)   { return MUSIC_ASSET_DIR   / filename; }
    path shader_asset_path(const path& filename)  { return SHADER_ASSET_DIR  / filename; }
    path sound_asset_path(const path& filename)   { return SOUND_ASSET_DIR   / filename; }
    path texture_asset_path(const path& filename) { return TEXTURE_ASSET_DIR / filename; }

    AssetManager::AssetManager(void)
    {
        DRK_LOG_CORE_TRACE("Initializing Audio system");
        Audio::init();
    }

    AssetManager::~AssetManager(void)
    {
        DRK_LOG_CORE_TRACE("Shutting down Audio system");
        Audio::shutdown();
    }
}
