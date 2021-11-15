#include "RunSettings.hpp"

#include "BuildSettings.hpp"
#include "Log.hpp"
#include "Assert.hpp"

#include <yaml-cpp/yaml.h>

#include <fstream>

namespace DrkCraft
{
    CommandLineOptions& CommandLineOptions::get_instance(void)
    {
        static CommandLineOptions s_commandLineOptions;
        return s_commandLineOptions;
    }

    void CommandLineOptions::parse_args(int argc, char* argv[])
    {
        for (int i = 0; i < argc; i++)
        {
            std::string_view arg{argv[i]};
            switch (i)
            {
                case 0:
                    get_instance().name = std::filesystem::path(arg).filename().string();
                    break;

                case 1:
                    if (arg == "--dev")
                        get_instance().set_mode(GameMode::Dev);
                    break;

                default:
                    break;
            }
        }
        get_instance().argc = argc;
        get_instance().argv = argv;
    }

    std::string_view CommandLineOptions::get_arg(int i)
    {
        DRK_ASSERT_DEBUG(i < get_instance().argc, "arg[{}] does not exist", i);
        return {get_instance().argv[i]};
    }

    std::string_view CommandLineOptions::get_program_name(void)
    {
        return get_instance().name;
    }

    GameMode CommandLineOptions::get_game_mode(void)
    {
        return get_instance().mode;
    }

    std::string_view CommandLineOptions::get_game_mode_str(void)
    {
        return get_instance().mode_str;
    }

    void CommandLineOptions::set_mode(GameMode mode)
    {
        switch (mode)
        {
            case GameMode::Dev:
            #if defined(DRK_EN_DEV_MODE)
                get_instance().mode = GameMode::Dev;
                get_instance().mode_str = "Developer";
            #else
                DRK_ASSERT_CORE(false, "This build does not support Dev mode. Aborting");
            #endif
                break;

            case GameMode::Player:
                get_instance().mode = GameMode::Player;
                get_instance().mode_str = "Player";
                break;

            default:
                DRK_ASSERT_DEBUG(false, "Unknown GameMode");
        }
    }

    std::string RuntimeSettings::s_filename;
    SettingsData RuntimeSettings::s_settings;

    void RuntimeSettings::load_from_file(std::string_view filename)
    {
        s_filename = filename;
        YAML::Node settings = YAML::LoadFile(s_filename);

        if (settings["fullscreen"]) s_settings.fullscreen = settings["fullscreen"].as<bool>();
    }

    void RuntimeSettings::save_to_file(void)
    {
        YAML::Emitter settings;
        settings << YAML::BeginMap;
        settings << YAML::Key << "fullscreen" << YAML::Value << s_settings.fullscreen;
        settings << YAML::EndMap;

        std::ofstream outfile(s_filename);
        outfile << settings.c_str();
    }

    SettingsData& RuntimeSettings::get(void)
    {
        return s_settings;
    }

    void RuntimeSettings::set(const SettingsData& settings)
    {
        s_settings = settings;
    }
}
