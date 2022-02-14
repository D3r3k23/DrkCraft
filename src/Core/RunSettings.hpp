#ifndef DRK_CORE_RUN_SETTINGS_HPP
#define DRK_CORE_RUN_SETTINGS_HPP

#include "Core/Base.hpp"
#include "System/Input.hpp"

#include "lib/fs.hpp"

#include <string>
#include <string_view>

namespace DrkCraft
{
    class CommandLineOptions
    {
    private:
        CommandLineOptions(void) = default;
        static CommandLineOptions& get_data(void);

    public:
        static void parse_args(int argc, char* argv[]);
        static std::string_view get_arg(int i);

        static std::string_view get_program_name(void);
        static bool dev_mode_activated(void);

    private:
        void activate_dev_mode(void);

    private:
        int argc = 0;
        char** argv = nullptr;

        // Options
        std::string name = "DrkCraft";
        bool dev = false; // Or maybe this should be runtime-enabled with a hotkey
    };

    struct ConfigData
    {
        struct
        {
            int width = 1280;
            int height = 720;
        } init_window_size;

        std::string saves_directory = "data/saves";
    };

    struct SettingsData
    {
        struct Video
        {
            bool fullscreen = false;
            int fullscreen_monitor = 0;
            bool vsync = true;
        } video;

        struct Audio
        {
            float volume = 0.5f;
            bool music = true;
        } audio;

        struct Controls
        {
            int sensitivity = 0.5f;
        } controls;
    };

    enum class Setting
    {
        Fullscreen,
        FullscreenMonitor,
        VSync,
        Volume,
        Music,
        Sensitivity
    };

    struct KeyBinds
    {
        struct PlayerMovement
        {
            KeyCode forward = KeyCode::W;
            KeyCode back    = KeyCode::S;
            KeyCode left    = KeyCode::A;
            KeyCode right   = KeyCode::D;

            KeyCode sprint = KeyCode::LeftShift;
            KeyCode crouch = KeyCode::LeftCtrl;
            KeyCode jump   = KeyCode::Space;

        } player_movement;

        struct PlayerActions
        {
            MouseCode use   = MouseCode::Button0; // Item: Weapon -> attack, Tool -> use, Block/other -> nothing
            MouseCode place = MouseCode::Button1; // Item: Weapon -> secondary, Tool -> nothing, Block/other -> place

            KeyCode interact  = KeyCode::E;
            KeyCode inventory = KeyCode::T;
            KeyCode fly       = KeyCode::F;
        } player_actions;
    };

    class RuntimeSettings
    {
    public:
        static void load(const fs::path& location);
        static void save(void);

        static const ConfigData& config(void);
        static const SettingsData& settings(void);
        static const KeyBinds& keybinds(void);

        static void set_settings(const SettingsData& settings);
        static void set_keybinds(const KeyBinds& keybinds);

    private:
        static void load_config_file(const fs::path& filename);
        static void load_settings_file(const fs::path& filename);
        static void load_keybinds_file(const fs::path& filename);

        static void save_config_file(const fs::path& filename);
        static void save_settings_file(const fs::path& filename);
        static void save_keybinds_file(const fs::path& filename);

    private:
        static fs::path s_configFile;
        static fs::path s_settingsFile;
        static fs::path s_keybindsFile;

        static ConfigData  s_configData;
        static SettingsData s_settingsData;
        static KeyBinds    s_keybindsData;
    };
}

#endif // DRK_CORE_RUN_SETTINGS_HPP
