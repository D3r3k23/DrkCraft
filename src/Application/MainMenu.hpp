#ifndef DRK_MAIN_MENU_HPP
#define DRK_MAIN_MENU_HPP

#include "Core/Base.hpp"
#include "Layer.hpp"
#include "Timestep.hpp"
#include "Events.hpp"
#include "SettingsMenu.hpp"

namespace DrkCraft
{
    class MainMenu : public Layer
    {
    public:
        MainMenu(void);
        ~MainMenu(void);

        void on_attach(void) override;
        void on_detach(void) override;

        void on_update(Timestep timestep) override;
        void on_render(Timestep timestep) override;
        void on_event(Event& event) override;

    private:
        bool on_key_pressed(KeyPressedEvent& event);
        bool on_mouse_button_pressed(MouseButtonPressedEvent& event);

    private:
        Ref<SettingsMenu> m_settingsMenu;
    };
}

#endif // DRK_MAIN_MENU_HPP
