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

    private:
        void set_mode(GameMode mode);

    private:
        int argc = 0;
        char** argv = nullptr;

        std::string name = "DrkCraft";
        GameMode mode = GameMode::Player; // Or maybe this should be runtime-enabled with a hotkey
    };

    struct SettingsData
    {
        bool fullscreen = false;
        int fullscreen_monitor = 0;
        bool vsync = true;
    };

    struct ConfigData
    {
        int init_window_width = 1280;
        int init_window_height = 720;

        std::string saves_directory = "data/saves";
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

    private:
        static std::string s_configFile;
        static std::string s_settingsFile;

        static ConfigData s_configData;
        static SettingsData s_settingsData;
    };
}

#endif // DRK_RUN_SETTINGS_HPP
