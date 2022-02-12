#ifndef DRK_CORE_RUN_SETTINGS_HPP
#define DRK_CORE_RUN_SETTINGS_HPP

#include "Core/Base.hpp"

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

    const uint NUM_SETTINGS = 3;

    enum SettingsEnum
    {
        Settings_Fullscreen,
        Settings_FullscreenMonitor,
        Settings_VSync
    };

    struct SettingsData
    {
        // Video
        bool fullscreen = false;
        int fullscreen_monitor = 0;
        bool vsync = true;

        // Audio
        float volume = 0.5f;
    };

    struct ConfigData
    {
        struct
        {
            int width = 1280;
            int height = 720;
        } init_window_size;

        fs::path saves_directory = "data/saves";
    };

    class RuntimeSettings
    {
    public:
        static void load(const fs::path& location);

        static void save_settings(void);

        static const ConfigData& config(void);

        static SettingsData& get(void);
        static void set(const SettingsData& settings);

    private:
        static void load_config(void);
        static void load_settings(void);

        static void save_config(void);

    private:
        static fs::path s_configFile;
        static fs::path s_settingsFile;

        static ConfigData s_configData;
        static SettingsData s_settingsData;
    };
}

#endif // DRK_CORE_RUN_SETTINGS_HPP
