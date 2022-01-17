#ifndef DRK_MAIN_MENU_HPP
#define DRK_MAIN_MENU_HPP

#include "Core/Base.hpp"
#include "Layer.hpp"
#include "Events.hpp"
#include "Core/Timestep.hpp"
#include "SettingsMenu.hpp"
#include "LoadingScreen.hpp"

namespace DrkCraft
{
    class MainMenu : public Layer
    {
    public:
        MainMenu(void);
        virtual ~MainMenu(void);

        void show_menu(void);
        void hide_menu(void);

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
        Ref<LoadingScreen> m_loadingScreen;
        bool m_show;
        bool m_applicationAssetsLoading;
        bool m_startButtonPushed;
    };
}

#endif // DRK_MAIN_MENU_HPP
