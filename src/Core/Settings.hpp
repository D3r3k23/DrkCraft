#ifndef DRK_CORE_SETTINGS_HPP
#define DRK_CORE_SETTINGS_HPP

#include "Core/Base.hpp"
#include "System/Input.hpp"

#include "lib/fs.hpp"

#include <string>
#include <string_view>

namespace DrkCraft
{
    // Usage:
    //   DrkCraft.exe [--dev]
    // Options:
    //   --dev  Enabled Dev mode

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
        int argc = 0;
        char** argv = nullptr;

        // Options
        std::string name = "DrkCraft";
        bool dev = false; // Or maybe this should be runtime-enabled with a hotkey
    };

    // These contain settings which are loaded from files

    struct ConfigData
    {
        struct InitWindowSize
        {
            int width = 1280;
            int height = 720;
        } init_window_size;

        std::string saves_directory = "data/saves";
        std::string screenshots_directory = "data/screenshots";
    };

    struct SettingsData
    {
        struct Video
        {
            bool fullscreen = false;
            int fs_monitor = 0;
            bool vsync = true;
            int fov = 50;
            int renderDistance;
        } video;

        struct Audio
        {
            int volume = 50;
            bool music = true;
        } audio;

        struct Controls
        {
            int sensitivity = 50;
        } controls;
    };

    struct KeyBinds
    {
        struct PlayerMovement
        {
            InputCode forward = KeyCode::W;
            InputCode back    = KeyCode::S;
            InputCode left    = KeyCode::A;
            InputCode right   = KeyCode::D;

            InputCode sprint = KeyCode::LeftShift;
            InputCode crouch = KeyCode::LeftCtrl;
            InputCode jump   = KeyCode::Space;

        } player_movement;

        struct PlayerActions
        {
            InputCode use   = MouseCode::Button0; // Item: Weapon -> attack, Tool -> use, Block/other -> nothing
            InputCode place = MouseCode::Button1; // Item: Weapon -> secondary, Tool -> nothing, Block/other -> place

            InputCode interact  = KeyCode::E;
            InputCode inventory = KeyCode::T;
            InputCode fly       = KeyCode::F;
        } player_actions;
    };

    enum class Setting
    {
        None = 0,

        // Video
        Fullscreen,
        FsMonitor,
        VSync,
        Fov,
        RenderDistance,

        // Audio
        Volume,
        Music,

        // Controls
        Sensitivity
    };

    class RuntimeSettings
    {
    private:
        static RuntimeSettings& get_instance(void);

    public:
        static void load(const fs::path& location);
        static void save(void);

        static const ConfigData& get_config(void);
        static const SettingsData& get_settings(void);
        static const KeyBinds& get_keybinds(void);

        static SettingsData& settings(void);
        static void set_settings(const SettingsData& settings);
        static void set_keybinds(const KeyBinds& keybinds);

    private:
        void load_config_file(void);
        void load_settings_file(void);
        void load_keybinds_file(void);

        void save_config_file(void);
        void save_settings_file(void);
        void save_keybinds_file(void);

    private:
        fs::path m_configFile;
        fs::path m_settingsFile;
        fs::path m_keybindsFile;

        ConfigData  m_configData;
        SettingsData m_settingsData;
        KeyBinds    m_keybindsData;
    };
}

#endif // DRK_CORE_SETTINGS_HPP
