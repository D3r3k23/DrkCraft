#include "SettingsMenu.hpp"

#include "Application/Application.hpp"
#include "Audio/Audio.hpp"
#include "System/Monitor.hpp"
#include "Application/ImGuiTools.hpp"
#include "System/Input.hpp"
#include "Core/Debug/Profiler.hpp"

#include "lib/glm/vec2.hpp"

#include <imgui/imgui.h>
#include <fmt/format.h>

#include <algorithm>

namespace DrkCraft
{
    SettingsMenu::SettingsMenu(bool activate)
      : Layer("SettingsMenuLayer", activate),
        m_settings(RuntimeSettings::settings()),
        m_dirty({false})
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

    void SettingsMenu::on_update(Timestep timestep)
    {

    }

    static vec2 WINDOW_SIZE(400, 600);

    void SettingsMenu::on_render(void)
    {
        DRK_PROFILE_FUNCTION();

        ImGuiTools::BeginCentered("Settings", WINDOW_SIZE, ImGuiWindowFlags_NoCollapse);
        ImGui::BeginGroup();

        if (ImGui::TreeNode("Video"))
        {
            if (ImGui::Checkbox("Fullscreen", &m_settings.video.fullscreen)) // Can crash if toggled without saving
                m_dirty[Settings_Fullscreen] = true;

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
            if (ImGui::BeginCombo("Fullscreen Monitor", monitorStrings[m_settings.video.fullscreen_monitor].c_str()))
            {
                for (int i = 0; const auto& monitor : monitors)
                {
                    // Is this right??
                    bool selected = (i == m_settings.video.fullscreen_monitor);
                    ImGui::Selectable(monitorStrings[i].c_str(), &selected);

                    if (selected)
                    {
                        m_settings.video.ullscreen_monitor = i;
                        m_dirty[Settings_FullscreenMonitor] = true;
                        ImGui::SetItemDefaultFocus();
                    }
                    i++;
                }
                ImGui::EndCombo();
            }
            if (ImGui::Checkbox("VSync", &m_settings.video.vsync))
                m_dirty[Settings_VSync] = true;
        }

        if (ImGui::TreeNode("Audio"))
        {
            if (ImGui::DragFloat("Volume", &m_settings.audio.volume))
                m_dirty[Settings_Volume] = true;

            if (ImGui::Checkbox("Music", &m_settings.audio.music))
                m_dirty[Settings_Music] = true;
        }

        if (ImGui::TreeNode("Controls"))
        {
            if (ImGui::DragFloat("Mouse Sensitivity", &m_settings.controls.sensitivity))
                m_dirty[Settings_Sensitivity] = true;
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

            for (uint i = 0; i < NUM_SETTINGS; i++)
                m_dirty[i] = false;
        }
        DRK_LOG_CORE_TRACE("SettingsMenu: Saved");
    }

    void SettingsMenu::apply(void)
    {
        DRK_PROFILE_FUNCTION();

        auto& app = Application::get_instance();
        if (m_dirty[Settings_Fullscreen])
        {
            if (m_settings.video.fullscreen)
                app.set_fullscreen(m_settings.video.fullscreen_monitor);
            else
                app.set_windowed();
        }
        if (m_dirty[Settings_FullscreenMonitor])
        {
            if (m_settings.video.fullscreen && !m_dirty[Settings_Fullscreen])
                app.set_fullscreen(m_settings.video.fullscreen_monitor);
        }
        if (m_dirty[Settings_VSync])
        {
            app.get_window().set_vsync(m_settings.video.vsync);
        }
        if (m_dirty[Settings_Volume])
        {
            Audio::set_volume(m_settings.audio.volume);
        }
        if (m_dirty[Settings_Music])
        {

        }
        if (m_dirty[Settings_Sensitivity])
        {

        }
    }

    void SettingsMenu::close(void)
    {
        DRK_LOG_CORE_TRACE("SettingsMenu closed");
        m_onClose();
        deactivate_layer();
    }
}