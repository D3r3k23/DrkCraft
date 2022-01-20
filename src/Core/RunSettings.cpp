#include "RunSettings.hpp"

#include "BuildSettings.hpp"
#include "Log.hpp"
#include "Assert.hpp"
#include "Core/Util.hpp"
#include "Core/Profiler.hpp"

#include <yaml-cpp/yaml.h>
#include <nameof.hpp>

#include <fstream>
#include <algorithm>

namespace DrkCraft
{
    ////////////////////////////////////
    //       CommandLineOptions       //
    ////////////////////////////////////

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
        DRK_PROFILE_FUNCTION();

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

    /////////////////////////////////
    //       RuntimeSettings       //
    /////////////////////////////////

    namespace fs = std::filesystem;

    fs::path RuntimeSettings::s_configFile;
    fs::path RuntimeSettings::s_settingsFile;

    ConfigData   RuntimeSettings::s_configData;
    SettingsData RuntimeSettings::s_settingsData;

    void RuntimeSettings::load(const fs::path& location)
    {
        DRK_PROFILE_FUNCTION();

        DRK_ASSERT_DEBUG(dir_exists(location), "Invalid config directory");
        auto defaultsLocation = location / "default";
        bool defaultsFound = dir_exists(defaultsLocation);

        s_configFile   = location / "config.yaml";
        s_settingsFile = location / "settings.yaml";

        if (!file_exists(s_configFile))
        {
            if (defaultsFound)
            {
                auto defaultConfigFile = defaultsLocation / "config.yaml";
                if (file_exists(defaultConfigFile))
                    fs::copy(defaultConfigFile, s_configFile);
                else
                    save_config();
            }
            else
                save_config();
        }
        load_config();

        if (!file_exists(s_settingsFile))
        {
            if (defaultsFound)
            {
                auto defaultSettingsFile = defaultsLocation / "settings.yaml";
                if (file_exists(defaultSettingsFile))
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
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_INFO("Saving settings to {}", s_settingsFile.generic_string());

        YAML::Emitter settings;
        settings << YAML::BeginMap;
        settings   << YAML::Key << "video" << YAML::BeginMap;
        settings     << YAML::Key << "fullscreen"         << YAML::Value << s_settingsData.fullscreen;
        settings     << YAML::Key << "fullscreen_monitor" << YAML::Value << s_settingsData.fullscreen_monitor;
        settings     << YAML::Key << "vsync"              << YAML::Value << s_settingsData.vsync;
        settings   << YAML::EndMap;
        settings   << YAML::Key << "audio" << YAML::BeginMap;
        settings     << YAML::Key << "volume" << YAML::Value << s_settingsData.volume;
        settings   << YAML::EndMap;
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
        DRK_PROFILE_FUNCTION();

        DRK_ASSERT_DEBUG(file_exists(s_configFile), "Config file not found");
        DRK_LOG_CORE_INFO("Loading config from {}", s_configFile.generic_string());
        YAML::Node config;
        try
        {
            config = YAML::LoadFile(s_configFile.string());
        }
        catch (const YAML::ParserException& e)
        {
            DRK_LOG_CORE_ERROR("Could not load config file");
            return;
        }
        if (!config.IsMap())
            DRK_LOG_CORE_WARN("Invalid config.yaml format");
        else
        {
            if (config["init_window_size"] && config["init_window_size"].IsMap())
            {
                auto initWindowSize = config["init_window_size"];
                if (initWindowSize["width"])
                {
                    s_configData.init_window_size.width  = initWindowSize["width"].as<int>(1280);
                }
                if (initWindowSize["height"])
                {
                    s_configData.init_window_size.height = initWindowSize["height"].as<int>(720);
                }
            }
            else
                DRK_LOG_CORE_WARN("Invalid config.yaml format \"init_window_size\"");

            if (config["saves_directory"])
            {
                s_configData.saves_directory = config["saves_directory"].as<std::string>("data/saves");
            }
        }
    }

    void RuntimeSettings::load_settings(void)
    {
        DRK_PROFILE_FUNCTION();

        DRK_ASSERT_DEBUG(file_exists(s_settingsFile), "Settings file not found");
        DRK_LOG_CORE_INFO("Loading settings from {}", s_settingsFile.generic_string());
        YAML::Node settings;
        try
        {
            settings = YAML::LoadFile(s_settingsFile.string());
        }
        catch (const YAML::ParserException& e)
        {
            DRK_LOG_CORE_ERROR("Could not load settings file");
            return;
        }
        if (!settings.IsMap())
            DRK_LOG_CORE_ERROR("Invalid settings.yaml format \".\"");
        else
        {
            if (settings["video"] && settings["video"].IsMap())
            {
                auto videoSettings = settings["video"];
                if (videoSettings["fullscreen"])
                {
                    s_settingsData.fullscreen = videoSettings["fullscreen"].as<bool>(false);
                }
                if (videoSettings["fullscreen_monitor"])
                {
                    if (int monitor = videoSettings["fullscreen_monitor"].as<int>(0); monitor >= 0)
                        s_settingsData.fullscreen_monitor = monitor;
                }
                if (videoSettings["vsync"])
                {
                    s_settingsData.vsync = videoSettings["vsync"].as<bool>(true);
                }
            }
            else
                DRK_LOG_CORE_WARN("Invalid settings.yaml format \"video\"");

            if (settings["audio"] && settings["audio"].IsMap())
            {
                auto audioSettings = settings["audio"];
                if (audioSettings["volume"])
                {
                    s_settingsData.volume = std::clamp(audioSettings["volume"].as<float>(0.5f), 0.0f, 1.0f);
                }
            }
            else
                DRK_LOG_CORE_ERROR("Invalid settings.yaml format \"video\"");
        }
    }

    void RuntimeSettings::save_config(void)
    {
        DRK_PROFILE_FUNCTION();
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
