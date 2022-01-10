#include "AssetManager.hpp"

#include <filesystem>

using std::filesystem::path;

static const path ASSET_DIR         = "assets";
static const path FONT_ASSET_DIR    = ASSET_DIR / "fonts";
static const path ICON_ASSET_DIR    = ASSET_DIR / "icons";
static const path IMAGE_ASSET_DIR   = ASSET_DIR / "images";
static const path MODEL_ASSET_DIR   = ASSET_DIR / "models";
static const path MUSIC_ASSET_DIR   = ASSET_DIR / "music";
static const path SHADER_ASSET_DIR  = ASSET_DIR / "shaders";
static const path SOUND_ASSET_DIR   = ASSET_DIR / "sounds";
static const path TEXTURE_ASSET_DIR = ASSET_DIR / "textures";

namespace DrkCraft
{
    std::string font_asset_path(std::string_view name)    { return (FONT_ASSET_DIR    / path(name)).generic_string(); }
    std::string icon_asset_path(std::string_view name)    { return (ICON_ASSET_DIR    / path(name)).generic_string(); }
    std::string image_asset_path(std::string_view name)   { return (IMAGE_ASSET_DIR   / path(name)).generic_string(); }
    std::string model_asset_path(std::string_view name)   { return (MODEL_ASSET_DIR   / path(name)).generic_string(); }
    std::string music_asset_path(std::string_view name)   { return (MUSIC_ASSET_DIR   / path(name)).generic_string(); }
    std::string shader_asset_path(std::string_view name)  { return (SHADER_ASSET_DIR  / path(name)).generic_string(); }
    std::string sound_asset_path(std::string_view name)   { return (SOUND_ASSET_DIR   / path(name)).generic_string(); }
    std::string texture_asset_path(std::string_view name) { return (TEXTURE_ASSET_DIR / path(name)).generic_string(); }
}
