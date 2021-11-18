#include "PauseMenu.hpp"

#include "Application/Application.hpp"
#include "Core/Profiler.hpp"

#include <imgui/imgui.h>

namespace DrkCraft
{
    PauseMenu::PauseMenu(bool activate)
      : Layer("PauseMenuLayer", activate),
        m_settingsMenu(Layer::create<SettingsMenu>(false))
    {
        m_settingsMenu->set_close_callback_fn(DRK_BIND_FN(activate_layer));
    }

    void PauseMenu::set_unpause_callback_fn(const PauseMenuUnpauseCallbackFn& fn)
    {
        m_onUnpause = fn;
    }

    void PauseMenu::set_exit_game_callback_fn(const PauseMenuExitGameCallbackFn& fn)
    {
        m_onExitGame = fn;
    }

    void PauseMenu::set_save_game_callback_fn(const PauseMenuSaveGameCallbackFn& fn)
    {
        m_onSaveGame = fn;
    }

    void PauseMenu::on_attach(void)
    {
        Application::get_instance().add_overlay(m_settingsMenu);
    }

    void PauseMenu::on_detach(void)
    {
        m_settingsMenu->detach_layer();
    }

    void PauseMenu::on_update(Timestep timestep)
    {

    }

    void PauseMenu::on_render(void)
    {
        DRK_PROFILE_FUNCTION();

        ImGui::PushFont(ImGuiManager::get_font(Font::Title));
        ImGuiTools::BeginFullscreen("Pause Menu", ImGuiWindowFlags_NoBackground);

        ImGui::Dummy({250, 50});
        ImGui::BeginGroup();

        if (ImGuiTools::ButtonCentered("Unpause", {250, 100}))
            unpause();

        ImGui::Dummy({250, 50});

        if (ImGuiTools::ButtonCentered("Settings", {250, 100}))
            open_settings();

        ImGui::Dummy({250, 50});

        if (ImGuiTools::ButtonCentered("Save Game", {250, 100}))
            save_game();

        ImGui::Dummy({250, 50});

        if (ImGuiTools::ButtonCentered("Exit", {250, 100}))
            exit_game();

        ImGui::EndGroup();

        ImGui::End();
        ImGui::PopFont();
    }

    void PauseMenu::on_event(Event& event)
    {
        EventDispatcher ed(event);
        ed.dispatch<KeyPressedEvent>(DRK_BIND_FN(on_key_pressed));
    }

    bool PauseMenu::on_key_pressed(const KeyPressedEvent& event)
    {
        switch (event.key)
        {
            case KeyCode::Escape:
            {
                unpause();
                return true;
            }
            default:
                return false;
        }
    }

    void PauseMenu::open_settings(void)
    {
        DRK_LOG_CORE_TRACE("PauseMenu: Opening Settings");
        m_settingsMenu->activate_layer();
        deactivate_layer();
    }

    void PauseMenu::save_game(void)
    {
        DRK_LOG_CORE_TRACE("PauseMenu: Saving Game");
        m_onSaveGame();
    }

    void PauseMenu::unpause(void)
    {
        DRK_LOG_CORE_TRACE("PauseMenu: Closing");
        m_onUnpause();
        detach_layer();
    }

    void PauseMenu::exit_game(void)
    {
        DRK_LOG_CORE_TRACE("PauseMenu: Exiting Game");
        // If not saved: pop up box
        m_onExitGame();
        detach_layer();
    }
}
