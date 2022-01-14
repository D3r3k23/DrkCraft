#ifndef DRK_MAIN_MENU_HPP
#define DRK_MAIN_MENU_HPP

#include "Core/Base.hpp"
#include "Layer.hpp"
#include "Events.hpp"
#include "Core/Timestep.hpp"
#include "SettingsMenu.hpp"

namespace DrkCraft
{
    class MainMenu : public Layer
    {
    public:
        MainMenu(void);
        virtual ~MainMenu(void);

        void show(void);
        void hide(void);

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_update(Timestep timestep) override;
        virtual void on_render(void) override;
        virtual void on_event(Event& event) override;

    private:
        void start_game(void);
        void open_settings(void);
        void exit(void);

    private:
        Ref<SettingsMenu> m_settingsMenu;
        bool m_show;
    };
}

#endif // DRK_MAIN_MENU_HPP
