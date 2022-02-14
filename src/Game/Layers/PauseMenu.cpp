#include "PauseMenu.hpp"

#include "Application/Application.hpp"
#include "Application/Layers/MainMenu.hpp"
#include "System/Input.hpp"
#include "Core/Debug/Profiler.hpp"

#include <imgui/imgui.h>

namespace DrkCraft
{
    PauseMenu::PauseMenu(bool activate)
      : Layer("PauseMenuLayer", activate),
        m_settingsMenu(Layer::create<SettingsMenu>(false)),
        m_options(
        {
            { "Unpause",      DRK_BIND_FN(unpause)       },
            { "Settings",     DRK_BIND_FN(open_settings) },
            { "Save Game",    DRK_BIND_FN(save_game)     },
            { "Exit Game",    DRK_BIND_FN(exit_game)     },
            { "Quit to Menu", DRK_BIND_FN(quit_to_menu)  },
            { "Unpause",      DRK_BIND_FN(unpause)       }
        })
    {
        m_settingsMenu->set_close_callback_fn(DRK_BIND_FN(activate_layer));
    }

    PauseMenu::~PauseMenu(void)
    {

    }

    void PauseMenu::set_unpause_callback_fn(const UnpauseCallbackFn& fn)
    {
        m_onUnpause = fn;
    }

    void PauseMenu::set_exit_game_callback_fn(const ExitGameCallbackFn& fn)
    {
        m_onExitGame = fn;
    }

    void PauseMenu::set_save_game_callback_fn(const SaveGameCallbackFn& fn)
    {
        m_onSaveGame = fn;
    }

    void PauseMenu::on_attach(void)
    {
        Application::add_layer(m_settingsMenu);
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

        ImGui::PushFont(ImGuiManager::get_font(ImGuiFont::Button));
        ImGuiTools::BeginFullscreen("Pause Menu", ImGuiWindowFlags_NoBackground);

        ImGui::BeginGroup();
        for (const auto& option : m_options)
        {
            ImGui::Dummy({250, 50});

            const auto& [str, action] = option;
            if (ImGuiTools::ButtonCentered(str, {250, 100}))
                action();
        }
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

    void PauseMenu::quit_to_menu(void)
    {
        DRK_LOG_CORE_TRACE("PauseMenu: Quitting to Menu");
        exit_game();
        Application::add_layer(Layer::create<MainMenu>());
    }
}
