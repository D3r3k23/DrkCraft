#include "RunSettings.hpp"

#include "BuildSettings.hpp"
#include "Log.hpp"
#include "Assert.hpp"
#include "Core/Util.hpp"

#include <yaml-cpp/yaml.h>
#include <nameof.hpp>

#include <fstream>

namespace DrkCraft
{
    //////// CommandLineOptions ////////

    std::string_view game_mode_to_string(GameMode mode)
    {
        const auto str = NAMEOF_ENUM(mode);
        DRK_ASSERT_DEBUG(str.length() > 0, "Unknown GameMode");
        return str;
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
            {
            #if DRK_DEV_MODE_ENABLED
                get_instance().mode = GameMode::Dev;
            #else
                DRK_ASSERT_CORE(false, "This build does not support Dev mode. Aborting");
            #endif
                break;
            }
            case GameMode::Player:
                get_instance().mode = GameMode::Player;
                break;

            default:
                DRK_ASSERT_DEBUG(false, "Unknown GameMode");
        }
    }

    //////// RuntimeSettings ////////

    namespace fs = std::filesystem;

    fs::path RuntimeSettings::s_configFile;
    fs::path RuntimeSettings::s_settingsFile;

    ConfigData   RuntimeSettings::s_configData;
    SettingsData RuntimeSettings::s_settingsData;

    void RuntimeSettings::load(const fs::path& location)
    {
        DRK_ASSERT_DEBUG(fs::is_directory(location), "Invalid config directory");
        auto defaultsLocation = location / "default";
        bool defaultsFound = fs::is_directory(defaultsLocation);

        s_configFile   = location / "config.yaml";
        s_settingsFile = location / "settings.yaml";

        if (!fs::is_regular_file(s_configFile))
        {
            if (defaultsFound)
            {
                auto defaultConfigFile = defaultsLocation / "config.yaml";
                if (fs::is_regular_file(defaultConfigFile))
                    fs::copy(defaultConfigFile, s_configFile);
                else
                    save_config();
            }
            else
                save_config();
        }
        load_config();

        if (!fs::is_regular_file(s_settingsFile))
        {
            if (defaultsFound)
            {
                auto defaultSettingsFile = defaultsLocation / "settings.yaml";
                if (fs::is_regular_file(defaultSettingsFile))
                    fs::copy(defaultSettingsFile, s_settingsFile);
                else
                    save_settings();
            }
            else
                save_settings();
        }
        load_settings();
    }

    void RuntimeSettings::save_settings(void)
    {
        DRK_LOG_CORE_INFO("Saving settings to {}", s_settingsFile.generic_string());

        YAML::Emitter settings;
        settings << YAML::BeginMap;
        settings   << YAML::Key << "fullscreen"         << YAML::Value << s_settingsData.fullscreen;
        settings   << YAML::Key << "fullscreen_monitor" << YAML::Value << s_settingsData.fullscreen_monitor;
        settings   << YAML::Key << "vsync"              << YAML::Value << s_settingsData.vsync;
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
        DRK_ASSERT_DEBUG(fs::is_regular_file(s_configFile), "Config file not found");
        DRK_LOG_CORE_INFO("Loading config from {}", s_configFile.generic_string());
        YAML::Node config = YAML::LoadFile(s_configFile.string());

        if (!config.IsMap())
            DRK_LOG_CORE_WARN("Invalid config.yaml format");
        else
        {
            if (config["init_window_size"])
            {
                YAML::Node initWindowSize = config["init_window_size"].as<YAML::Node>();
                if (!initWindowSize.IsMap())
                    DRK_LOG_CORE_WARN("Invalid config.yaml format");
                else
                {
                    s_configData.init_window_size.width  = initWindowSize["width"].as<int>();
                    s_configData.init_window_size.height = initWindowSize["height"].as<int>();
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
        DRK_ASSERT_DEBUG(fs::is_regular_file(s_settingsFile), "Settings file not found");
        DRK_LOG_CORE_INFO("Loading settings from {}", s_settingsFile.generic_string());
        YAML::Node settings = YAML::LoadFile(s_settingsFile.string());

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

    void RuntimeSettings::save_config(void)
    {
        DRK_LOG_CORE_INFO("Saving config to {}", s_configFile.generic_string());

        YAML::Emitter config;
        config << YAML::BeginMap;
        config   << YAML::Key << "init_window_size";
        config   << YAML::BeginMap;
        config     << YAML::Key << "width"  << YAML::Value << s_configData.init_window_size.width;
        config     << YAML::Key << "height" << YAML::Value << s_settingsData.fullscreen_monitor;
        config   << YAML::EndMap;
        config   << YAML::Key << "saves_directory" << YAML::Value << s_configData.saves_directory.generic_string();
        config << YAML::EndMap;

        std::ofstream outfile(s_configFile);
        outfile << config.c_str();
    }
}
