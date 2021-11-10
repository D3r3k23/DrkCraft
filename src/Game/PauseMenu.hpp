#ifndef DRK_PAUSE_MENU_HPP
#define DRK_PAUSE_MENU_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Timestep.hpp"
#include "Application/Events.hpp"

namespace DrkCraft
{
    class PauseMenu : public Layer
    {
    public:
        PauseMenu(void);

        void on_update(Timestep timestep) override;
        void on_render(Timestep timestep) override;
        void on_event(Event& event) override;

        bool on_mouse_button_pressed(MouseButtonPressedEvent& event);

    private:

    };
}

#endif // DRK_PAUSE_MENU_HPP
