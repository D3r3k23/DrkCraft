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

    CommandLineOptions& CommandLineOptions::get_data(void)
    {
        static CommandLineOptions s_instance;
        return s_instance;
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
                    get_data().name = fs::path(arg).filename().string();
                    break;

                case 1:
                    if (arg == "--dev")
                        get_data().activate_dev_mode();
                    break;

                default:
                    break;
            }
        }
        get_data().argc = argc;
        get_data().argv = argv;
    }

    std::string_view CommandLineOptions::get_arg(int i)
    {
        DRK_ASSERT_DEBUG(i < get_data().argc, "arg[{}] does not exist", i);
        return get_data().argv[i];
    }

    std::string_view CommandLineOptions::get_program_name(void)
    {
        return get_data().name;
    }

    bool CommandLineOptions::dev_mode_activated(void)
    {
        return get_data().dev;
    }

    void CommandLineOptions::activate_dev_mode(void)
    {
        if constexpr (DRK_DEV_MODE_ENABLED)
            get_data().dev = true;
        else
            DRK_ASSERT_CORE(false, "This build does not support Dev mode. Aborting");
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

    void RuntimeSettings::load(const fs::path& location)
    {
        DRK_PROFILE_FUNCTION();

        DRK_ASSERT_DEBUG(is_dir(location), "Invalid config directory");

        get_instance().m_configFile   = location / "config.yaml";
        get_instance().m_settingsFile = location / "settings.yaml";
        get_instance().m_keybindsFile = location / "keybinds.yaml";

        const auto defaultsLocation = location / "default";

        // For settings, config, & keybinds:
        // If file not found: Copy default file if it exists
        const char* settings_config_keybinds[] {
            "config.yaml",
            "settings.yaml",
            "keybinds.yaml",
        };
        for (const char* filename : settings_config_keybinds)
        {
            if (!is_file(location / filename))
            {
                auto defaultFilename = defaultsLocation / filename;
                if (is_file(defaultFilename))
                    fs::copy(defaultFilename, filename);
            }
        }
        if (!is_file(get_instance().m_configFile)) // Save defaults, since this is not modified or saved later
            get_instance().save_config_file();
        else
            get_instance().load_config_file();

        get_instance().load_settings_file();
        get_instance().load_keybinds_file();
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
        template <typename T, typename RepT=T>
        using SettingTransform = std::function<T(RepT)>;

        template <typename T>
        SettingTransform<T> apply_range(T min, T max)
        {
            return [min, max](T value) -> T
            {
                return std::clamp(value, min, max);
            };
        }

        template <typename T>
        SettingTransform<T> apply_min(T min)
        {
            return [min](T value) -> T
            {
                return std::max(value, min);
            }
        }

        template <typename T>
        SettingTransform<T> apply_max(T max)
        {
            return [max](T value) -> T
            {
                return std::min(value, max);
            }
        }

        static SettingTransform<InputCode, std::string> keybind_converter = [](std::string name) -> InputCode
        {
            return to_input_code(name);
        };

        static SettingTransform<KeyCode, std::string> key_code_converter = [](std::string name) -> KeyCode
        {
            return to_key_code(name);
        };

        static SettingTransform<MouseCode, std::string> mouse_code_converter = [](std::string name) -> MouseCode
        {
            return to_mouse_code(name);
        };

        template <typename T>
        void load_setting(const YAML::Node& parent, std::string_view key, T& value)
        {
            if (const auto setting = YAML::get_scalar(parent, key); value.has_value())
                value = setting->as<T>(value);
        }

        template <typename T, typename RepT=T>
        void load_setting(const YAML::Node& parent, std::string_view key, T& value,
            const SettingTransform<T, RepT>& transform)
        {
            if (const auto setting = Yaml::get_scalar(parent, key); setting.has_value())
                value = transform(setting->as<RepT>(value));
        }
    }

    void RuntimeSettings::load_config_file(void)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_CORE_INFO("Loading config from {}", m_configFile.generic_string());
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
        load_setting(config, "saves_directory", m_configData.saves_directory);

        ensure_dir_exists(m_configData.saves_directory);
    }

    void RuntimeSettings::load_settings_file(void)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_CORE_INFO("Loading settings from {}", m_settingsFile.generic_string());
        auto document = Yaml::load(m_settingsFile);
        if (!document)
            return;
        auto settings = *document;

        if (const auto map = Yaml::get_map(settings, "video"); map.has_value())
        {
            auto& videoSettings = m_settingsData.video;
            load_setting(*map, "fullscreen", videoSettings.fullscreen);
            load_setting(*map, "fs_monitor", videoSettings.fs_monitor, apply_min(0));
            load_setting(*map, "vsync",      videoSettings.vsync);
            load_setting(*map, "fov",        videoSettings.fov, apply_range(0, 100));
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
        auto document = Yaml::load(m_keybindsFile);
        if (!document)
            return;
        auto keybinds = *document;

        if (const auto map = Yaml::get_map(keybinds, "player_movement"); map.has_value())
        {
            auto& movementKeybinds = m_keybindsData.player_movement;
            load_setting(*map, "forward", movementKeybinds.forward, keybind_converter);
            load_setting(*map, "back",    movementKeybinds.back,   keybind_converter);
            load_setting(*map, "left",    movementKeybinds.left,  keybind_converter);
            load_setting(*map, "right",   movementKeybinds.right,  keybind_converter);
            load_setting(*map, "sprint",  movementKeybinds.sprint,  keybind_converter);
            load_setting(*map, "crouch",  movementKeybinds.crouch, keybind_converter);
            load_setting(*map, "jump",    movementKeybinds.jump,  keybind_converter);
        }
        if (const auto map = Yaml::get_map(keybinds, "player_actions"); map.has_value())
        {
            auto& actionKeybinds = m_keybindsData.player_actions;
            load_setting(*map, "use",    actionKeybinds.use,     keybind_converter);
            load_setting(*map, "place",   actionKeybinds.place,    keybind_converter);
            load_setting(*map, "interact", actionKeybinds.interact,  keybind_converter);
            load_setting(*map, "inventory", actionKeybinds.inventory, keybind_converter);
            load_setting(*map, "fly",        actionKeybinds.fly,      keybind_converter);
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
        config   << YAML::Key << "saves_directory" << YAML::Value << m_configData.saves_directory;
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
        settings     << YAML::Key << "fullscreen" << YAML::Value << video.fullscreen;
        settings     << YAML::Key << "fs_monitor" << YAML::Value << video.fs_monitor;
        settings     << YAML::Key << "vsync"      << YAML::Value << video.vsync;
        settings     << YAML::Key << "fov"        << YAML::Value << video.fov;
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
