#include "RunSettings.hpp"

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

    fs::path RuntimeSettings::s_configFile;
    fs::path RuntimeSettings::s_settingsFile;
    fs::path RuntimeSettings::s_keybindsFile;

    ConfigData   RuntimeSettings::s_configData;
    SettingsData RuntimeSettings::s_settingsData;
    KeyBinds     RuntimeSettings::s_keybindsData;

    void RuntimeSettings::load(const fs::path& location)
    {
        DRK_PROFILE_FUNCTION();

        DRK_ASSERT_DEBUG(is_dir(location), "Invalid config directory");

        s_configFile   = location / "config.yaml";
        s_settingsFile = location / "settings.yaml";
        s_keybindsFile = location / "keybinds.yaml";

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
        if (!is_file(s_configFile)) // Save defaults, since this is not modified or saved later
            save_config_file(s_configFile);

        load_config_file(s_configFile);
        load_settings_file(s_settingsFile);
        load_keybinds_file(s_keybindsFile);
    }

    void RuntimeSettings::save(void)
    {
        DRK_PROFILE_FUNCTION();

        save_settings_file(s_settingsFile);
        save_keybinds_file(s_keybindsFile);
    }

    const ConfigData& RuntimeSettings::config(void)
    {
        return s_configData;
    }

    const SettingsData& RuntimeSettings::settings(void)
    {
        return s_settingsData;
    }

    const KeyBinds& RuntimeSettings::keybinds(void)
    {
        return s_keybindsData;
    }

    void RuntimeSettings::set_settings(const SettingsData& settings)
    {
        s_settingsData = settings;
    }

    void RuntimeSettings::set_keybinds(const KeyBinds& keybinds)
    {
        s_keybindsData = keybinds;
    }

    namespace
    {
        template <typename T, typename RepT=T>
        using SettingTransform = std::function<T(RepT)>;

        template <typename T, typename RepT=T>
        void assign_if_setting_is_valid(const YAML::Node& node, std::string_view key, T& value,
            const SettingTransform<T, RepT>& transform={})
        {
            if (node[key])
            {
                if (transform)
                    value = transform(node[key].as<RepT>(value));
                else
                    value = node[key].as<RepT>(value);
            }
        }

        template <typename T>
        const SettingTransform<T>& setting_range(T min, T max)
        {
            static const auto fn = [min, max](T value) -> T
            {
                return std::clamp(value, min, max);
            };
            return fn;
        }

        template <typename T>
        const SettingTransform<T>& setting_min(T min)
        {
            static const auto fn = [min](T value) -> T
            {
                return std::max(value, min);
            }
            return fn;
        }

        template <typename T>
        const SettingTransform<T>& setting_max(T max)
        {
            static const auto fn = [max](T value) -> T
            {
                return std::min(value, max);
            }
            return fn;
        }

        const SettingTransform<KeyCode, std::string>& key_code_converter(void) // string_view?
        {
            static const auto fn = [](std::string name) -> KeyCode
            {
                return to_key_code(name);
            };
            return fn;
        }

        const SettingTransform<MouseCode, std::string>& mouse_code_converter(void) // string_view?
        {
            static const auto fn = [](std::string name) -> MouseCode
            {
                return to_mouse_code(name);
            };
            return fn;
        }
    }

    void RuntimeSettings::load_config_file(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_CORE_INFO("Loading config from {}", filename.generic_string());
        auto document = Yaml::load(filename);
        if (!document)
            return;
        auto config = *document;

        if (Yaml::check_map(config, "init_window_size"))
        {
            auto& initWindowSize = s_configData.init_window_size;

            assign_if_setting_is_valid(config["init_window_size"], "width",  initWindowSize.width,  setting_min(0));
            assign_if_setting_is_valid(config["init_window_size"], "height", initWindowSize.height, setting_min(0));
        }
        assign_if_setting_is_valid(config, "saves_directory", s_configData.saves_directory);

        ensure_dir_exists(s_configData.saves_directory);
    }

    void RuntimeSettings::load_settings_file(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_CORE_INFO("Loading settings from {}", filename.generic_string());
        auto document = Yaml::load(filename);
        if (!document)
            return;
        auto settings = *document;

        if (Yaml::check_map(settings, "video"))
        {
            auto& videoSettings = s_settingsData.video;

            assign_if_setting_is_valid(settings["video"], "fullscreen",         videoSettings.fullscreen);
            assign_if_setting_is_valid(settings["video"], "fullscreen_monitor", videoSettings.fullscreen_monitor, setting_min(0));
            assign_if_setting_is_valid(settings["video"], "vsync",              videoSettings.vsync);
        }
        if (Yaml::check_map(settings, "audio"))
        {
            auto& audioSettings = s_settingsData.audio;

            assign_if_setting_is_valid(settings["audio"], "volume", audioSettings.volume, setting_range(0.0f, 1.0f));
            assign_if_setting_is_valid(settings["audio"], "music",  audioSettings.music);
        }
        if (Yaml::check_map(settings, "controls"))
        {
            auto& controlsSettings = s_settingsData.controls;

            assign_if_setting_is_valid(settings["controls"], "sensitivity", controlsSettings.sensitivity, setting_range(0, 100));
        }
    }

    void RuntimeSettings::load_keybinds_file(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_CORE_INFO("Loading keybinds from {}", filename.generic_string());
        auto document = Yaml::load(filename);
        if (!document)
            return;
        auto keybinds = *document;

        if (Yaml::check_map(keybinds, "player_movement"))
        {
            auto& movementKeybinds = s_keybindsData.player_movement;

            assign_if_setting_is_valid(keybinds["player_movement"], "forward", movementKeybinds.forward, key_code_converter());
            assign_if_setting_is_valid(keybinds["player_movement"], "back",    movementKeybinds.back,    key_code_converter());
            assign_if_setting_is_valid(keybinds["player_movement"], "left",    movementKeybinds.left,    key_code_converter());
            assign_if_setting_is_valid(keybinds["player_movement"], "right",   movementKeybinds.right,   key_code_converter());

            assign_if_setting_is_valid(keybinds["player_movement"], "sprint", movementKeybinds.sprint, key_code_converter());
            assign_if_setting_is_valid(keybinds["player_movement"], "crouch", movementKeybinds.crouch, key_code_converter());
            assign_if_setting_is_valid(keybinds["player_movement"], "jump",   movementKeybinds.jump,   key_code_converter());
        }
        if (Yaml::check_map(keybinds, "player_actions"))
        {
            auto& actionKeybinds = s_keybindsData.player_actions;

            assign_if_setting_is_valid(keybinds["player_actions"], "use",    actionKeybinds.use,    mouse_code_converter());
            assign_if_setting_is_valid(keybinds["player_actions"], "place",   actionKeybinds.place, mouse_code_converter());
            assign_if_setting_is_valid(keybinds["player_actions"], "interact", actionKeybinds.interact,   key_code_converter());
            assign_if_setting_is_valid(keybinds["player_actions"], "inventory", actionKeybinds.inventory, key_code_converter());
            assign_if_setting_is_valid(keybinds["player_actions"], "fly",        actionKeybinds.fly,      key_code_converter());
        }
    }

    void RuntimeSettings::save_config_file(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_INFO("Saving config to {}", filename.generic_string());

        YAML::Emitter config;
        config << YAML::BeginMap;
        config   << YAML::Key << "init_window_size";
        config   << YAML::BeginMap;
        config     << YAML::Key << "width"  << YAML::Value << s_configData.init_window_size.width;
        config     << YAML::Key << "height" << YAML::Value << s_configData.init_window_size.height;
        config   << YAML::EndMap;
        config   << YAML::Key << "saves_directory" << YAML::Value << s_configData.saves_directory;
        config << YAML::EndMap;

        write_file(filename, config.c_str());
    }

    void RuntimeSettings::save_settings_file(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_INFO("Saving settings to {}", filename.generic_string());

        const auto& video  = s_settingsData.video;
        const auto& audio   = s_settingsData.audio;
        const auto& controls = s_settingsData.controls;

        YAML::Emitter settings;
        settings << YAML::BeginMap;
        settings   << YAML::Key << "video";
        settings   << YAML::BeginMap;
        settings     << YAML::Key << "fullscreen"         << YAML::Value << video.fullscreen;
        settings     << YAML::Key << "fullscreen_monitor" << YAML::Value << video.fullscreen_monitor;
        settings     << YAML::Key << "vsync"              << YAML::Value << video.vsync;
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

        write_file(filename, settings.c_str());
    }

    void RuntimeSettings::save_keybinds_file(const fs::path& filename)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_INFO("Saving keybinds to {}", filename.generic_string());

        const auto& movement = s_keybindsData.player_movement;
        const auto& actions  = s_keybindsData.player_actions;

        YAML::Emitter keybinds;
        keybinds << YAML::BeginMap;
        keybinds   << YAML::Key << "player_movement";
        keybinds   << YAML::BeginMap;
        keybinds     << YAML::Key << "forward" << YAML::Value << key_code_name(movement.forward);
        keybinds     << YAML::Key << "back"    << YAML::Value << key_code_name(movement.back);
        keybinds     << YAML::Key << "left"    << YAML::Value << key_code_name(movement.left);
        keybinds     << YAML::Key << "right"   << YAML::Value << key_code_name(movement.right);
        keybinds     << YAML::Key << "sprint"  << YAML::Value << key_code_name(movement.sprint);
        keybinds     << YAML::Key << "crouch"  << YAML::Value << key_code_name(movement.crouch);
        keybinds     << YAML::Key << "jump"    << YAML::Value << key_code_name(movement.jump);
        keybinds   << YAML::EndMap;
        keybinds   << YAML::Key << "player_actions";
        keybinds   << YAML::BeginMap;
        keybinds     << YAML::Key << "use"       << YAML::Value << mouse_code_name(actions.use);
        keybinds     << YAML::Key << "place"     << YAML::Value << mouse_code_name(actions.place);
        keybinds     << YAML::Key << "interact"  << YAML::Value << key_code_name(actions.interact);
        keybinds     << YAML::Key << "inventory" << YAML::Value << key_code_name(actions.inventory);
        keybinds     << YAML::Key << "fly"       << YAML::Value << key_code_name(actions.fly);
        keybinds   << YAML::EndMap;
        keybinds << YAML::EndMap;

        write_file(filename, keybinds.c_str());
    }
}
