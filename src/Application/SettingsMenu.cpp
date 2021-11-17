#include "SettingsMenu.hpp"

#include "Application.hpp"
#include "Monitor.hpp"
#include "ImGuiTools.hpp"
#include "Core/Profiler.hpp"

#include <imgui/imgui.h>
#include <glm/vec2.hpp>

#include <format>

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

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize;
        // ImGui::SetNextWindowSize(WINDOW_SIZE);
        float x = (ImGui::GetWindowContentRegionMax().x - WINDOW_SIZE.x) * 0.5f;
        float y = (ImGui::GetCursorPos().y + WINDOW_SIZE.y) * 0.5;
        ImGui::SetNextWindowPos({x, y});

        ImGui::Begin("SettingsMenu", nullptr, windowFlags);
        ImGuiTools::TextCentered("Settings");

        ImGui::BeginGroup();

        if (ImGui::Checkbox("Fullscreen", &m_settings->fullscreen))
            m_dirty = true;

        static int selectedMonitorIndex = 0;
        auto monitors = Monitor::get_monitors();
        if (ImGui::BeginCombo("Fullscreen Monitor", "example"))
        {
            for (int i = 0; const auto& monitor : monitors)
            {
                const auto& res = monitor.get_resolution();
                const auto& rRate = monitor.get_refresh_rate();
                const auto& name = monitor.get_name();
                auto monitorStr = std::format("{}: {}x{} {}hz ({})", i, res.x, res.y, rRate, name);
                if (ImGui::Selectable(monitorStr.c_str(), i == selectedMonitorIndex));
                    selectedMonitorIndex = i;
                i++;
            }
            ImGui::EndCombo();
        }
        if (ImGui::Checkbox("VSync", &m_settings->vsync))
            m_dirty = true;

        ImGui::EndGroup();

        ImGui::BeginGroup();

        if (ImGui::Button("Close", {60, 30}))
            close();

        ImGui::SameLine();

        if (ImGui::Button("Save", {60, 30}))
            save();

        ImGui::SameLine();

        if (ImGui::Button("Save & Close", {60, 30}))
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
