#include "RunSettings.hpp"

#include "BuildSettings.hpp"
#include "Log.hpp"
#include "Assert.hpp"
#include "Core/Util.hpp"

#include <yaml-cpp/yaml.h>

#include <fstream>

namespace DrkCraft
{
    //////// CommandLineOptions ////////

    std::string_view game_mode_to_string(GameMode mode)
    {
        switch (mode)
        {
            case GameMode::Dev    : return "dev";
            case GameMode::Player : return "player";
            default:
                DRK_ASSERT_DEBUG(false, "Unknown GameMode");
                return "";
        }
    }

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

    void CommandLineOptions::set_mode(GameMode mode)
    {
        switch (mode)
        {
            case GameMode::Dev:
            #if defined(DRK_EN_DEV_MODE)
                get_instance().mode = GameMode::Dev;
            #else
                DRK_ASSERT_CORE(false, "This build does not support Dev mode. Aborting");
            #endif
                break;

            case GameMode::Player:
                get_instance().mode = GameMode::Player;
                break;

            default:
                DRK_ASSERT_DEBUG(false, "Unknown GameMode");
        }
    }

    //////// RuntimeSettings ////////

    std::string RuntimeSettings::s_configFile;
    std::string RuntimeSettings::s_settingsFile;

    ConfigData   RuntimeSettings::s_configData;
    SettingsData RuntimeSettings::s_settingsData;

    using std::filesystem::path;

    void RuntimeSettings::load(const path& location)
    {
        s_configFile   = (location / path("config.yaml")).string();
        s_settingsFile = (location / path("settings.yaml")).string();

        load_config();
        load_settings();
    }

    void RuntimeSettings::save_settings(void)
    {
        DRK_LOG_CORE_TRACE("Saving settings to file");

        YAML::Emitter settings;
        settings << YAML::BeginMap;
        settings << YAML::Key   << "fullscreen";
        settings << YAML::Value << s_settingsData.fullscreen;
        settings << YAML::Key   << "fullscreen_monitor";
        settings << YAML::Value << s_settingsData.fullscreen_monitor;
        settings << YAML::Key   << "vsync";
        settings << YAML::Value << s_settingsData.vsync;
        settings << YAML::EndMap;

        std::ofstream outfile(s_settingsFile);
        outfile << settings.c_str();
    }

    const ConfigData& RuntimeSettings::config(void)
    {
        return s_configData;
    }

    SettingsData& RuntimeSettings::get(void)
    {
        return s_settingsData;
    }

    void RuntimeSettings::set(const SettingsData& settings)
    {
        s_settingsData = settings;
    }

    void RuntimeSettings::load_config(void)
    {
        YAML::Node config = YAML::LoadFile(s_configFile);

        if (!config.IsMap())
            DRK_LOG_CORE_WARN("Invalid config.yaml format");
        else
        {
            if (config["init_window_size"])
            {
                YAML::Node initWindowSize = config["init_window_size"].as<YAML::Node>();
                if (!initWindowSize.IsMap())
                    DRK_LOG_CORE_WARN("Invalid config.yaml format: {}", "init_window_size");
                else
                {
                    s_configData.init_window_width  = initWindowSize["width"].as<int>();
                    s_configData.init_window_height = initWindowSize["height"].as<int>();
                }
            }
            if (config["saves_directory"])
            {
                s_configData.saves_directory = config["saves_directory"].as<std::string>();
            }
        }
    }

    void RuntimeSettings::load_settings(void)
    {
        YAML::Node settings = YAML::LoadFile(s_settingsFile);

        if (!settings.IsMap())
            DRK_LOG_CORE_WARN("Invalid settings.yaml format");
        else
        {
            if (settings["fullscreen"])
            {
                s_settingsData.fullscreen = settings["fullscreen"].as<bool>();
            }
            if (settings["fullscreen_monitor"])
            {
                if (settings["fullscreen_monitor"].IsDefined())
                    if (int monitor = settings["fullscreen_monitor"].as<int>(); monitor >= 0)
                        s_settingsData.fullscreen_monitor = monitor;
            }
            if (settings["vsync"])
            {
                s_settingsData.vsync = settings["vsync"].as<bool>();
            }
        }
    }
}
