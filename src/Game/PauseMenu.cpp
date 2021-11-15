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

    }

    PauseMenu::~PauseMenu(void)
    {

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
        m_settingsMenu->set_close_callback_fn([&]
        {
            activate_layer();
        });
        Application::get_instance().add_overlay(m_settingsMenu);
    }

    void PauseMenu::on_detach(void)
    {
        m_settingsMenu->detach_layer();
    }

    void PauseMenu::on_update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();
    }

    void PauseMenu::on_render(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();
    }

    void PauseMenu::on_event(Event& event)
    {
        DRK_PROFILE_FUNCTION();

        EventDispatcher ed(event);
        ed.dispatch<MouseButtonPressedEvent>(DRK_BIND_FN(on_mouse_button_pressed));
        ed.dispatch<KeyPressedEvent>(DRK_BIND_FN(on_key_pressed));
    }

    bool PauseMenu::on_key_pressed(KeyPressedEvent& event)
    {
        switch (event.key)
        {
            case KeyCode::Escape:
            {
                m_onUnpause();
                detach_layer();
                return true;
            }
            case KeyCode::Delete:
            {
                m_onExitGame();
                detach_layer();
                return true;
            }
            case KeyCode::Enter:
            {
                m_onSaveGame();
                return true;
            }
            default:
                return false;
        }
    }

    bool PauseMenu::on_mouse_button_pressed(MouseButtonPressedEvent& event)
    {
        if (event.button == MouseCode::Left)
        {
            if (1)
            {
                m_settingsMenu->activate_layer();
                deactivate_layer();
                return true;
            }

            // If Save & Exit button pressed:
            // Application::get_instance().stop_game();
            // Application::get_instance().get_layer_stack().remove(this);
            return true;
        }
        else
            return false;
    }
}
