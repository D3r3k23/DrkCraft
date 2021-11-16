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

        void show(void);
        void hide(void);

        void on_attach(void) override;
        void on_detach(void) override;

        void on_update(Timestep timestep) override;
        void on_render(Timestep timestep) override;
        void on_event(Event& event) override;

    private:
        bool on_key_pressed(const KeyPressedEvent& event);

        void start_game(void);
        void open_settings(void);
        void exit(void);

    private:
        Ref<SettingsMenu> m_settingsMenu;
        bool m_show;
    };
}

#endif // DRK_MAIN_MENU_HPP
