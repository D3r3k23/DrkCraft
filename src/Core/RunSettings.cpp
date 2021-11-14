#include "RunSettings.hpp"

#include "BuildSettings.hpp"
#include "Log.hpp"
#include "Assert.hpp"

#include <string>
#include <filesystem>

namespace DrkCraft
{
    struct CommandLineOptions
    {
        std::string name = "DrkCraft";

        GameMode mode = GameMode::Player; // Or maybe this should be enabled with a hotkey
        std::string mode_str = "Player";
    };

    static CommandLineOptions s_commandLineOptions;

    static int s_argc = 0;
    static char** s_argv = nullptr;

    void RunSettings::parse_args(int argc, char* argv[])
    {
        for (int i = 0; i < argc; i++)
        {
            std::string_view arg{argv[i]};
            switch (i)
            {
                case 0:
                    s_commandLineOptions.name = std::filesystem::path(arg).filename().string();
                    break;

                case 1:
                    if (arg == "--dev")
                        set_mode(GameMode::Dev);
                    break;

                default:
                    break;
            }
        }
        s_argc = argc;
        s_argv = argv;
    }

    std::string_view RunSettings::get_arg(int i)
    {
        DRK_ASSERT_DEBUG(i < s_argc, "arg[{}] does not exist", i);
        return {s_argv[i]};
    }

    std::string_view RunSettings::get_program_name(void)
    {
        return s_commandLineOptions.name;
    }

    GameMode RunSettings::get_game_mode(void)
    {
        return s_commandLineOptions.mode;
    }

    std::string RunSettings::get_game_mode_str(void)
    {
        return s_commandLineOptions.mode_str;
    }

    void RunSettings::set_mode(GameMode mode)
    {
        switch (mode)
        {
            case GameMode::Dev:
            #if defined(DRK_EN_DEV_MODE)
                s_commandLineOptions.mode = GameMode::Dev;
                s_commandLineOptions.mode_str = "Developer";
            #else
                DRK_ASSERT_CORE(false, "This build does not support Dev mode. Aborting");
            #endif
                break;

            case GameMode::Player:
                s_commandLineOptions.mode = GameMode::Player;
                s_commandLineOptions.mode_str = "Player";
                break;

            default:
                DRK_ASSERT_DEBUG(false, "Unknown GameMode");
        }
    }
}
