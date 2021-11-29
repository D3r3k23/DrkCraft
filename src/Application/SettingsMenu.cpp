#include "SettingsMenu.hpp"

#include "Application.hpp"
#include "Monitor.hpp"
#include "ImGuiTools.hpp"
#include "Core/Profiler.hpp"

#include <imgui/imgui.h>
#include <glm/vec2.hpp>
#include <fmt/format.h>

namespace DrkCraft
{
    SettingsMenu::SettingsMenu(bool activate)
      : Layer("SettingsMenuLayer", activate),
        m_dirty(false)
    { }

    void SettingsMenu::set_close_callback_fn(const SettingsMenuCloseCallbackFn& fn)
    {
        m_onClose = fn;
    }

    void SettingsMenu::on_attach(void)
    {
        DRK_PROFILE_FUNCTION();

        m_settings = make_ptr<SettingsData>(RuntimeSettings::get());
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

        if (ImGui::Checkbox("Fullscreen", &m_settings->fullscreen))
            m_dirty = true;

        auto monitors = Monitor::get_monitors();
        std::vector<std::string> monitorStrings;
        for (int i = 0; const auto& monitor : monitors)
        {
            const auto& res = monitor.get_resolution();
            const auto& rRate = monitor.get_refresh_rate();
            const auto& name = monitor.get_name();
            monitorStrings.push_back(fmt::format("{}: {}x{} {}hz ({})", i, res.x, res.y, rRate, name));
            i++;
        }
        if (ImGui::BeginCombo("Fullscreen Monitor", monitorStrings[m_settings->fullscreen_monitor].c_str()))
        {
            for (int i = 0; const auto& monitor : monitors)
            {
                // Is this right??
                bool selected = (i == m_settings->fullscreen_monitor);
                if (ImGui::Selectable(monitorStrings[i].c_str(), &selected));
                if (selected)
                {
                    m_settings->fullscreen_monitor = i;
                    m_dirty = true;
                    ImGui::SetItemDefaultFocus();
                }
                i++;
            }
            ImGui::EndCombo();
        }
        if (ImGui::Checkbox("VSync", &m_settings->vsync))
            m_dirty = true;

        ImGui::EndGroup();
        ImGui::BeginGroup();

        if (ImGui::Button("Close", {80, 40}))
            close();

        ImGui::SameLine();

        if (ImGui::Button("Save", {80, 40}))
            save();

        ImGui::SameLine();

        if (ImGui::Button("Save & Close", {80, 40}))
        {
            save();
            close();
        }

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

    void SettingsMenu::apply(void)
    {
        if (m_dirty)
        {
            auto& window = Application::get_instance().get_window();
            if (m_settings->fullscreen)
                window.set_fullscreen();
            else
                window.set_windowed();

            if (window.is_fullscreen())
                window.change_fullscreen_monitor(m_settings->fullscreen_monitor);

            if (m_settings->vsync)
                window.set_vsync(true);
            else
                window.set_vsync(false);
        }
    }

    void SettingsMenu::save(void)
    {
        apply();
        DRK_LOG_CORE_TRACE("SettingsMenu: Saved");
        if (m_dirty)
            RuntimeSettings::set(*m_settings);
        m_dirty = false;
    }

    void SettingsMenu::close(void)
    {
        DRK_LOG_CORE_TRACE("SettingsMenu closed");
        m_onClose();
        deactivate_layer();
    }
}
