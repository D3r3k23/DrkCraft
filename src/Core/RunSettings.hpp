#ifndef DRK_RUN_SETTINGS_HPP
#define DRK_RUN_SETTINGS_HPP

#include <string>
#include <string_view>

namespace DrkCraft
{
    enum class GameMode
    {
        Dev,
        Player
    };

    class CommandLineOptions
    {
    private:
        CommandLineOptions(void) = default;
        static CommandLineOptions& get_instance(void);

    public:
        static void parse_args(int argc, char* argv[]);
        static std::string_view get_arg(int i);

        static std::string_view get_program_name(void);
        static GameMode         get_game_mode(void);
        static std::string_view get_game_mode_str(void);

    private:
        void set_mode(GameMode mode);

    private:
        int argc = 0;
        char** argv = nullptr;

        std::string name = "DrkCraft";

        GameMode mode = GameMode::Player; // Or maybe this should be runtime-enabled with a hotkey
        std::string mode_str = "Player";
    };

    struct SettingsData
    {
        bool fullscreen = false;
    };

    class RuntimeSettings
    {
    public:
        static void load_from_file(std::string_view filename);
        static void save_to_file(void);

        static SettingsData& get(void);
        static void set(const SettingsData& settings);

    private:
        static std::string  s_filename;
        static SettingsData s_settings;
    };
}

#endif // DRK_RUN_SETTINGS_HPP
