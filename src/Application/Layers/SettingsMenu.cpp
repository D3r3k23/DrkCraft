#include "SettingsMenu.hpp"

#include "Application/Application.hpp"
#include "Util/ImGui.hpp"
#include "Audio/Audio.hpp"
#include "System/Monitor.hpp"
#include "System/Input.hpp"
#include "Core/Debug/Profiler.hpp"

#include "lib/glm/vec2.hpp"

#include <imgui/imgui.h>
#include <fmt/format.h>

#include <algorithm>

namespace DrkCraft
{
    static constexpr uint NUM_SETTINGS = magic_enum::enum_count<Setting>();

    SettingsMenu::SettingsMenu(bool activate)
      : Layer("SettingsMenuLayer", activate),
        m_settings(RuntimeSettings::get_settings()),
        m_dirty(NUM_SETTINGS, false),
        m_keybinds(RuntimeSettings::get_keybinds()),
        m_keybindsDirty(false)
    { }

    SettingsMenu::~SettingsMenu(void)
    {

    }

    void SettingsMenu::set_close_callback_fn(const CloseCallbackFn& fn)
    {
        m_onClose = fn;
    }

    void SettingsMenu::on_attach(void)
    {

    }

    void SettingsMenu::on_detach(void)
    {

    }

    static vec2 WINDOW_SIZE(400, 600);

    void SettingsMenu::on_render(void)
    {
        DRK_PROFILE_FUNCTION();

        DrkImGui::BeginCentered("Settings", WINDOW_SIZE, ImGuiWindowFlags_NoCollapse);
        ImGui::BeginGroup();

        if (ImGui::TreeNode("Video"))
        {
            if (ImGui::Checkbox("Fullscreen", &m_settings.video.fullscreen)) // Can crash if toggled without saving
                make_dirty(Setting::Fullscreen);

            const auto& monitors = Application::get_monitors().monitors();
            std::vector<std::string> monitorStrings;
            for (int i = 0; const auto& monitor : monitors)
            {
                const auto& res   = monitor.get_resolution();
                const auto& rRate = monitor.get_refresh_rate();
                const auto& name  = monitor.get_name();
                monitorStrings.push_back(fmt::format("{}: {}x{} {}hz ({})", i, res.x, res.y, rRate, name));
                i++;
            }
            if (ImGui::BeginCombo("Fullscreen Monitor", monitorStrings[m_settings.video.fs_monitor].c_str()))
            {
                for (int i = 0; const auto& monitor : monitors)
                {
                    // Is this right??
                    bool selected = (i == m_settings.video.fs_monitor);
                    ImGui::Selectable(monitorStrings[i].c_str(), &selected);

                    if (selected)
                    {
                        m_settings.video.fs_monitor = i;
                        make_dirty(Setting::FsMonitor);
                        ImGui::SetItemDefaultFocus();
                    }
                    i++;
                }
                ImGui::EndCombo();
            }
            if (ImGui::Checkbox("VSync", &m_settings.video.vsync))
                make_dirty(Setting::VSync);

            if (ImGui::DragInt("Field of View", &m_settings.video.fov, 1.0f, 0, 100))
                make_dirty(Setting::Fov);
        }

        if (ImGui::TreeNode("Audio"))
        {
            if (ImGui::DragInt("Volume", &m_settings.audio.volume, 1.0f, 0, 100))
                make_dirty(Setting::Volume);

            if (ImGui::Checkbox("Music", &m_settings.audio.music))
                make_dirty(Setting::Music);
        }

        if (ImGui::TreeNode("Controls"))
        {
            if (ImGui::DragInt("Mouse Sensitivity", &m_settings.controls.sensitivity, 1.0f, 0, 100))
                make_dirty(Setting::Sensitivity);
        }

        ImGui::EndGroup();
        ImGui::BeginGroup();

        if (ImGui::Button("Save", {80, 40}))
            save();

        ImGui::SameLine();
        if (ImGui::Button("Save & Close", {80, 40}))
        {
            save();
            close();
        }
        ImGui::SameLine();
        if (ImGui::Button("Close", {80, 40}))
            close();

        ImGui::EndGroup();
        ImGui::End();
    }

    void SettingsMenu::on_update(Timestep timestep)
    {

    }

    void SettingsMenu::on_event(Event& event)
    {
        EventDispatcher ed(event);
        ed.dispatch<KeyPressedEvent>(DRK_BIND_FN(on_key_pressed));
    }

    bool SettingsMenu::on_key_pressed(const KeyPressedEvent& event)
    {
        switch (event.key)
        {
            case KeyCode::Enter:
                save();
                return true;

            case KeyCode::Escape:
                close();
                return true;

            default:
                return false;
        }
    }

    void SettingsMenu::save(void)
    {
        DRK_PROFILE_FUNCTION();

        if (std::ranges::any_of(m_dirty, [](bool dirty) { return dirty; }));
        {
            apply();
            RuntimeSettings::set_settings(m_settings);

            for (uint i = 0; i < m_dirty.size(); i++)
                m_dirty[i] = false;
        }
        if (m_keybindsDirty)
        {
            RuntimeSettings::set_keybinds(m_keybinds);
        }
        DRK_LOG_CORE_TRACE("SettingsMenu: Saved");
    }

    void SettingsMenu::apply(void)
    {
        DRK_PROFILE_FUNCTION();

        auto& app = Application::get_instance();
        if (dirty(Setting::Fullscreen))
        {
            if (m_settings.video.fullscreen)
                app.set_fullscreen(m_settings.video.fs_monitor);
            else
                app.set_windowed();
        }
        if (dirty(Setting::FsMonitor))
        {
            if (m_settings.video.fullscreen && !dirty(Setting::Fullscreen))
                app.set_fullscreen(m_settings.video.fs_monitor);
        }
        if (dirty(Setting::VSync))
        {
            app.get_window().set_vsync(m_settings.video.vsync);
        }
        if (dirty(Setting::Fov))
        {

        }
        if (dirty(Setting::Volume))
        {
            Audio::set_volume(m_settings.audio.volume);
        }
        if (dirty(Setting::Music))
        {

        }
        if (dirty(Setting::Sensitivity))
        {

        }
    }

    void SettingsMenu::close(void)
    {
        DRK_LOG_CORE_TRACE("SettingsMenu closed");
        m_onClose();
        deactivate_layer();
    }

    void SettingsMenu::make_dirty(Setting setting)
    {
        m_dirty[magic_enum::enum_integer(setting)] = true;
    }

    bool SettingsMenu::dirty(Setting setting) const
    {
        return m_dirty[magic_enum::enum_integer(setting)];
    }
}
