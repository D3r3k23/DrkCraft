#include "SettingsMenu.hpp"

#include "Application.hpp"
#include "System/Monitor.hpp"
#include "ImGuiTools.hpp"
#include "System/Input.hpp"
#include "Core/Profiler.hpp"

#include <imgui/imgui.h>
#include <glm/vec2.hpp>
#include <fmt/format.h>

#include <algorithm>

namespace DrkCraft
{
    SettingsMenu::SettingsMenu(bool activate)
      : Layer("SettingsMenuLayer", activate),
        m_settings(RuntimeSettings::get()),
        m_dirty({false})
    { }

    SettingsMenu::~SettingsMenu(void)
    {

    }

    void SettingsMenu::set_close_callback_fn(const SettingsMenuCloseCallbackFn& fn)
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

    static glm::vec2 WINDOW_SIZE(400, 600);

    void SettingsMenu::on_render(void)
    {
        DRK_PROFILE_FUNCTION();

        ImGuiTools::BeginCentered("Settings", WINDOW_SIZE, ImGuiWindowFlags_NoCollapse);
        ImGui::BeginGroup();

        if (ImGui::Checkbox("Fullscreen", &m_settings.fullscreen)) // Can crash if toggled without saving
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
        if (ImGui::BeginCombo("Fullscreen Monitor", monitorStrings[m_settings.fullscreen_monitor].c_str()))
        {
            for (int i = 0; const auto& monitor : monitors)
            {
                // Is this right??
                bool selected = (i == m_settings.fullscreen_monitor);
                ImGui::Selectable(monitorStrings[i].c_str(), &selected);

                if (selected)
                {
                    m_settings.fullscreen_monitor = i;
                    m_dirty[Settings_FullscreenMonitor] = true;
                    ImGui::SetItemDefaultFocus();
                }
                i++;
            }
            ImGui::EndCombo();
        }
        if (ImGui::Checkbox("VSync", &m_settings.vsync))
            m_dirty[Settings_VSync] = true;

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
            RuntimeSettings::set(m_settings);

            for (uint i = 0; i < NUM_SETTINGS; i++)
                m_dirty[i] = false;

            RuntimeSettings::save_settings();
        }
        DRK_LOG_CORE_TRACE("SettingsMenu: Saved");
    }

    void SettingsMenu::apply(void)
    {
        DRK_PROFILE_FUNCTION();

        auto& app = Application::get_instance();
        if (m_dirty[Settings_Fullscreen])
        {
            if (m_settings.fullscreen)
                app.set_fullscreen(m_settings.fullscreen_monitor);
            else
                app.set_windowed();
        }
        if (m_dirty[Settings_FullscreenMonitor])
        {
            if (m_settings.fullscreen && !m_dirty[Settings_Fullscreen])
                app.set_fullscreen(m_settings.fullscreen_monitor);
        }
        if (m_dirty[Settings_VSync])
        {
            app.get_window().set_vsync(m_settings.vsync);
        }
    }

    void SettingsMenu::close(void)
    {
        DRK_LOG_CORE_TRACE("SettingsMenu closed");
        m_onClose();
        deactivate_layer();
    }
}
