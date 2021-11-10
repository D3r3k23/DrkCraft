#include "PauseMenu.hpp"

namespace DrkCraft
{
    PauseMenu::PauseMenu(void)
      : Layer("PauseMenuLayer")
    {

    }

    void PauseMenu::on_update(Timestep timestep)
    {

    }

    void PauseMenu::on_render(Timestep timestep)
    {

    }

    void PauseMenu::on_event(Event& event)
    {
        EventDispatcher ed(event);
        ed.dispatch<MouseButtonPressedEvent>(DRK_BIND_EVENT_HANDLER(on_mouse_button_pressed));
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
