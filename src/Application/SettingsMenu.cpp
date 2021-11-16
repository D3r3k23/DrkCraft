#include "SettingsMenu.hpp"

#include "ImGuiTools.hpp"
#include "Core/Profiler.hpp"

#include <imgui/imgui.h>
#include <glm/vec2.hpp>

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

    void SettingsMenu::on_update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();


    }

    static glm::vec2 WINDOW_SIZE(400, 600);

    void SettingsMenu::on_render(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize;
        // ImGui::SetNextWindowSize(WINDOW_SIZE);
        float x = (ImGui::GetWindowContentRegionMax().x - WINDOW_SIZE.x) * 0.5f;
        // float y = (ImGui::GetCursorPos().y + WINDOW_SIZE.y) * 0.5;
        // ImGui::SetNextWindowPos({x, y});

        ImGui::Begin("SettingsMenu", nullptr, windowFlags);

        ImGuiTools::TextCentered("Settings");

        if (ImGui::Checkbox("Fullscreen", &m_settings->fullscreen))
            m_dirty = true;

        ImGui::BeginGroup();

        if (ImGui::Button("Cancel", {60, 30}))
            close();

        ImGui::SameLine();

        if (ImGui::Button("Apply", {60, 30}))
            save();

        ImGui::SameLine();

        if (ImGui::Button("Save", {60, 30}))
        {
            save();
            close();
        }

        ImGui::EndGroup();

        ImGui::End();
    }

    void SettingsMenu::on_event(Event& event)
    {
        DRK_PROFILE_FUNCTION();

        EventDispatcher ed(event);
        ed.dispatch<KeyPressedEvent>(DRK_BIND_FN(on_key_pressed));
    }

    bool SettingsMenu::on_key_pressed(KeyPressedEvent& event)
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
        DRK_LOG_CORE_TRACE("SettingsMenu: Saved");
        if (m_dirty)
        {
            RuntimeSettings::set(*m_settings);
            RuntimeSettings::save_to_file();
        }
        m_dirty = false;
    }

    void SettingsMenu::close(void)
    {
        DRK_LOG_CORE_TRACE("SettingsMenu closed");
        m_onClose();
        deactivate_layer();
    }
}
