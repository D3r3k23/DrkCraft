#ifndef DRK_APPLICATION_LAYERS_MAIN_MENU_HPP
#define DRK_APPLICATION_LAYERS_MAIN_MENU_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Events.hpp"
#include "Application/Timestep.hpp"

#include "Application/Layers/SettingsMenu.hpp"
#include "Application/Layers/LoadingScreen.hpp"

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

        virtual void on_render(void) override;
        virtual void on_update(Timestep timestep) override;
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

#endif // DRK_APPLICATION_LAYERS_MAIN_MENU_HPP
