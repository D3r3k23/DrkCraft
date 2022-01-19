#ifndef DRK_RUN_SETTINGS_HPP
#define DRK_RUN_SETTINGS_HPP

#include <string>
#include <string_view>
#include <filesystem>

namespace DrkCraft
{
    enum class GameMode
    {
        Dev,
        Player
    };

    std::string_view game_mode_to_string(GameMode mode);

    class CommandLineOptions
    {
    private:
        CommandLineOptions(void) = default;
        static CommandLineOptions& get_instance(void);

    public:
        static void parse_args(int argc, char* argv[]);
        static std::string_view get_arg(int i);

        static std::string_view get_program_name(void);
        static GameMode get_game_mode(void);
        static bool get_trace_log(void);

    private:
        void set_mode(GameMode mode);

    private:
        int argc = 0;
        char** argv = nullptr;

        std::string name = "DrkCraft";
        GameMode mode = GameMode::Player; // Or maybe this should be runtime-enabled with a hotkey
    };

    const unsigned int NUM_SETTINGS = 3;

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

        std::filesystem::path saves_directory = "data/saves";
    };

    class RuntimeSettings
    {
    public:
        static void load(const std::filesystem::path& location);

        static void save_settings(void);

        static const ConfigData& config(void);

        static SettingsData& get(void);
        static void set(const SettingsData& settings);

    private:
        static void load_config(void);
        static void load_settings(void);

        static void save_config(void);

    private:
        static std::filesystem::path s_configFile;
        static std::filesystem::path s_settingsFile;

        static ConfigData s_configData;
        static SettingsData s_settingsData;
    };
}

#endif // DRK_RUN_SETTINGS_HPP
