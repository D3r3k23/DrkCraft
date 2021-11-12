#ifndef DRK_RUN_SETTINGS_HPP
#define DRK_RUN_SETTINGS_HPP

#include <string_view>

namespace DrkCraft
{
    enum class GameMode
    {
        Dev,
        Player
    };

    class RunSettings
    {
    public:
        static void parse_args(int argc, char* argv[]);
        std::string_view get_arg(int i);

        static std::string_view get_program_name(void);
        static GameMode get_game_mode(void);
        static std::string get_game_mode_str(void);
    };
}

#endif // DRK_COMMANDRK_RUN_SETTINGS_HPPD_LINE_ARGS_HPP
