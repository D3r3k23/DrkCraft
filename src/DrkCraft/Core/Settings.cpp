#include "Settings.hpp"

#include "Util/Yaml.hpp"
#include "Util/File.hpp"
#include "Core/Debug/Profiler.hpp"

#include <yaml-cpp/yaml.h>

#include <algorithm>
#include <functional>

namespace DrkCraft
{
    ////////////////////////////////////
    //       CommandLineOptions       //
    ////////////////////////////////////

    OptionsData CommandLineOptions::s_options;

    const OptionsData& CommandLineOptions::parse(const Argv::Argv& argv)
    {
        DRK_PROFILE_FUNCTION();

        DRK_ASSERT_DEBUG_NO_MSG(argv.count() > 0);
        s_options.program = argv[0];

        if (argv.count() > 1)
        {
            if constexpr (DRK_DEV_MODE_ENABLED)
            {
                if (argv.contains("--dev"))
                    s_options.en_dev_mode = true;
            }

            if constexpr (DRK_LOGGING_ENABLED)
            {
                if (argv.contains("--trace"))
                    s_options.en_trace_log = true;
            }
        }
    }

    const OptionsData& CommandLineOptions::get_options(void)
    {
        return s_options;
    }

    void CommandLineOptions::log_options(void)
    {
        if (s_options.en_dev_mode)
            DRK_LOG_CORE_INFO("Dev mode enabled");

        if (s_options.en_trace_log)
            DRK_LOG_CORE_INFO("Console trace logging enabled");
    }

    /////////////////////////////////
    //       RuntimeSettings       //
    /////////////////////////////////

    ////// Static //////

    RuntimeSettings& RuntimeSettings::get_instance(void)
    {
        static RuntimeSettings s_instance;
        return s_instance;
    }

    void RuntimeSettings::load(const fs::path& location) // Remove default config files, since config structs already have defaults
    {
        DRK_PROFILE_FUNCTION();

        DRK_ASSERT_DEBUG(is_dir(location), "Config directory does not exist");

        get_instance().m_configFile   = location / "config.yaml";
        get_instance().m_settingsFile = location / "settings.yaml";
        get_instance().m_keybindsFile = location / "keybinds.yaml";

        if (is_file(get_instance().m_configFile))
            get_instance().load_config_file();
        else
            get_instance().save_config_file();

        if (is_file(get_instance().m_settingsFile))
            get_instance().load_settings_file();
        else
            get_instance().save_settings_file();

        if (is_file(get_instance().m_keybindsFile))
            get_instance().load_keybinds_file();
        else
            get_instance().save_keybinds_file();
    }

    void RuntimeSettings::save(void)
    {
        DRK_PROFILE_FUNCTION();

        get_instance().save_settings_file();
        get_instance().save_keybinds_file();
    }

    const ConfigData& RuntimeSettings::get_config(void)
    {
        return get_instance().m_configData;
    }

    const SettingsData& RuntimeSettings::get_settings(void)
    {
        return get_instance().m_settingsData;
    }

    const KeyBinds& RuntimeSettings::get_keybinds(void)
    {
        return get_instance().m_keybindsData;
    }

    SettingsData& RuntimeSettings::settings(void)
    {
        return get_instance().m_settingsData;
    }

    void RuntimeSettings::set_settings(const SettingsData& settings)
    {
        get_instance().m_settingsData = settings;
    }

    void RuntimeSettings::set_keybinds(const KeyBinds& keybinds)
    {
        get_instance().m_keybindsData = keybinds;
    }

    ////// Instance //////

    namespace
    {
        template <typename T>
        using SettingConstraint = std::function<T(T)>;

        template <typename T>
        SettingConstraint<T> apply_range(T min, T max)
        {
            return [min, max](T value) -> T
            {
                return std::clamp(value, min, max);
            };
        }

        template <typename T>
        SettingConstraint<T> apply_min(T min)
        {
            return [min](T value) -> T
            {
                return std::max(value, min);
            };
        }

        template <typename T>
        SettingConstraint<T> apply_max(T max)
        {
            return [max](T value) -> T
            {
                return std::min(value, max);
            };
        }

        template <typename T>
        void load_setting(const YAML::Node& parent, const string& key, T& setting, const SettingConstraint<T>& constraint={})
        {
            if (const auto scalar = Yaml::get_scalar(parent, key); scalar.has_value())
            {
                if (const auto value = Yaml::get_value<T>(*scalar); value)
                {
                    if (constraint)
                        setting = constraint(*value);
                    else
                        setting = *value;
                }
            }
        }

        void load_keybind(const YAML::Node& parent, const string& key, InputCode& keybind)
        {
            if (const auto scalar = Yaml::get_scalar(parent, key); scalar.has_value())
            {
                if (const auto value = Yaml::get_value<string>(*scalar); value)
                    keybind = to_input_code(*value);
            }
        }
    }

    void RuntimeSettings::load_config_file(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_INFO("Loading config from {}", m_configFile.generic_string());
        DRK_ASSERT_DEBUG(is_file(m_configFile), "{} does not exist", m_configFile.generic_string());

        auto document = Yaml::load(m_configFile);
        if (!document)
            return;
        auto config = *document;

        if (const auto map = Yaml::get_map(config, "init_window_size"); map.has_value())
        {
            auto& initWindowSizeSetting = m_configData.init_window_size;
            load_setting(*map, "width",  initWindowSizeSetting.width,  apply_min(0));
            load_setting(*map, "height", initWindowSizeSetting.height, apply_min(0));
        }
        load_setting(config, "screenshots_directory", m_configData.screenshots_directory);
    }

    void RuntimeSettings::load_settings_file(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_INFO("Loading settings from {}", m_settingsFile.generic_string());
        DRK_ASSERT_DEBUG(is_file(m_settingsFile), "{} does not exist", m_settingsFile.generic_string());

        auto document = Yaml::load(m_settingsFile);
        if (!document)
            return;
        auto settings = *document;

        if (const auto map = Yaml::get_map(settings, "video"); map.has_value())
        {
            auto& videoSettings = m_settingsData.video;
            load_setting(*map, "fullscreen",  videoSettings.fullscreen);
            load_setting(*map, "fs_monitor",   videoSettings.fs_monitor, apply_min(0));
            load_setting(*map, "vsync",         videoSettings.vsync);
            load_setting(*map, "fov",            videoSettings.fov, apply_range(0, 100));
            load_setting(*map, "render_distance", videoSettings.render_distance, apply_range(4, 64));
        }
        if (const auto map = Yaml::get_map(settings, "audio"); map.has_value())
        {
            auto& audioSettings = m_settingsData.audio;
            load_setting(*map, "volume", audioSettings.volume, apply_range(0, 100));
            load_setting(*map, "music",  audioSettings.music);
        }
        if (const auto map = Yaml::get_map(settings, "controls"); map.has_value())
        {
            auto& controlsSettings = m_settingsData.controls;
            load_setting(*map, "sensitivity", controlsSettings.sensitivity, apply_range(0, 100));
        }
    }

    void RuntimeSettings::load_keybinds_file(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_INFO("Loading keybinds from {}", m_keybindsFile.generic_string());
        DRK_ASSERT_DEBUG(is_file(m_keybindsFile), "{} does not exist", m_keybindsFile.generic_string());

        auto document = Yaml::load(m_keybindsFile);
        if (!document)
            return;
        auto keybinds = *document;

        if (const auto map = Yaml::get_map(keybinds, "player_movement"); map.has_value())
        {
            auto& movementKeybinds = m_keybindsData.player_movement;
            load_keybind(*map, "forward", movementKeybinds.forward);
            load_keybind(*map, "back",    movementKeybinds.back);
            load_keybind(*map, "left",    movementKeybinds.left);
            load_keybind(*map, "right",   movementKeybinds.right);
            load_keybind(*map, "sprint",  movementKeybinds.sprint);
            load_keybind(*map, "crouch",  movementKeybinds.crouch);
            load_keybind(*map, "jump",    movementKeybinds.jump);
        }
        if (const auto map = Yaml::get_map(keybinds, "player_actions"); map.has_value())
        {
            auto& actionKeybinds = m_keybindsData.player_actions;
            load_keybind(*map, "use",    actionKeybinds.use);
            load_keybind(*map, "place",   actionKeybinds.place);
            load_keybind(*map, "interact", actionKeybinds.interact);
            load_keybind(*map, "inventory", actionKeybinds.inventory);
            load_keybind(*map, "fly",        actionKeybinds.fly);
        }
    }

    void RuntimeSettings::save_config_file(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_INFO("Saving config to {}", m_configFile.generic_string());

        YAML::Emitter config;
        config << YAML::BeginMap;
        config   << YAML::Key << "init_window_size";
        config   << YAML::BeginMap;
        config     << YAML::Key << "width"  << YAML::Value << m_configData.init_window_size.width;
        config     << YAML::Key << "height" << YAML::Value << m_configData.init_window_size.height;
        config   << YAML::EndMap;
        config   << YAML::Key << "screenshots_directory" << YAML::Value << m_configData.screenshots_directory;
        config << YAML::EndMap;

        write_file(m_configFile, config.c_str());
    }

    void RuntimeSettings::save_settings_file(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_INFO("Saving settings to {}", m_settingsFile.generic_string());

        const auto& video  = m_settingsData.video;
        const auto& audio   = m_settingsData.audio;
        const auto& controls = m_settingsData.controls;

        YAML::Emitter settings;
        settings << YAML::BeginMap;
        settings   << YAML::Key << "video";
        settings   << YAML::BeginMap;
        settings     << YAML::Key << "fullscreen"      << YAML::Value << video.fullscreen;
        settings     << YAML::Key << "fs_monitor"      << YAML::Value << video.fs_monitor;
        settings     << YAML::Key << "vsync"           << YAML::Value << video.vsync;
        settings     << YAML::Key << "fov"             << YAML::Value << video.fov;
        settings     << YAML::Key << "render_distance" << YAML::Value << video.render_distance;
        settings   << YAML::EndMap;
        settings   << YAML::Key << "audio";
        settings   << YAML::BeginMap;
        settings     << YAML::Key << "volume" << YAML::Value << audio.volume;
        settings     << YAML::Key << "music"  << YAML::Value << audio.music;
        settings   << YAML::EndMap;
        settings   << YAML::Key << "controls";
        settings   << YAML::BeginMap;
        settings     << YAML::Key << "sensitivity" << YAML::Value << controls.sensitivity;
        settings   << YAML::EndMap;
        settings << YAML::EndMap;

        write_file(m_settingsFile, settings.c_str());
    }

    void RuntimeSettings::save_keybinds_file(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_INFO("Saving keybinds to {}", m_keybindsFile.generic_string());

        const auto& movement = m_keybindsData.player_movement;
        const auto& actions  = m_keybindsData.player_actions;

        YAML::Emitter keybinds;
        keybinds << YAML::BeginMap;
        keybinds   << YAML::Key << "player_movement";
        keybinds   << YAML::BeginMap;
        keybinds     << YAML::Key << "forward" << YAML::Value << input_code_name(movement.forward).data();
        keybinds     << YAML::Key << "back"    << YAML::Value << input_code_name(movement.back).data();
        keybinds     << YAML::Key << "left"    << YAML::Value << input_code_name(movement.left).data();
        keybinds     << YAML::Key << "right"   << YAML::Value << input_code_name(movement.right).data();
        keybinds     << YAML::Key << "sprint"  << YAML::Value << input_code_name(movement.sprint).data();
        keybinds     << YAML::Key << "crouch"  << YAML::Value << input_code_name(movement.crouch).data();
        keybinds     << YAML::Key << "jump"    << YAML::Value << input_code_name(movement.jump).data();
        keybinds   << YAML::EndMap;
        keybinds   << YAML::Key << "player_actions";
        keybinds   << YAML::BeginMap;
        keybinds     << YAML::Key << "use"       << YAML::Value << input_code_name(actions.use).data();
        keybinds     << YAML::Key << "place"     << YAML::Value << input_code_name(actions.place).data();
        keybinds     << YAML::Key << "interact"  << YAML::Value << input_code_name(actions.interact).data();
        keybinds     << YAML::Key << "inventory" << YAML::Value << input_code_name(actions.inventory).data();
        keybinds     << YAML::Key << "fly"       << YAML::Value << input_code_name(actions.fly).data();
        keybinds   << YAML::EndMap;
        keybinds << YAML::EndMap;

        write_file(m_keybindsFile, keybinds.c_str());
    }
}
