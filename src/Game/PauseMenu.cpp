#include "PauseMenu.hpp"

#include "Application/SettingsMenu.hpp"
#include "Core/Profiler.hpp"

#include <imgui/imgui.h>

namespace DrkCraft
{
    PauseMenu::PauseMenu(bool activate)
      : Layer("PauseMenuLayer", activate)
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

    }

    void PauseMenu::on_detach(void)
    {

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
        switch (event.button)
        {
            case MouseCode::Left:
            {
                // If Save & Exit button pressed:
                // Application::get_instance().stop_game();
                // Application::get_instance().get_layer_stack().remove(this);
                return true;
            }
            default:
                return false;
        }
    }
}
