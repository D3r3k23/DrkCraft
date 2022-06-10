#ifndef DRK_GAME_LAYERS_PAUSE_MENU_HPP
#define DRK_GAME_LAYERS_PAUSE_MENU_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Events.hpp"
#include "Util/Timestep.hpp"

#include "Application/Layers/SettingsMenu.hpp"

#include <unordered_map>
#include <functional>

namespace DrkCraft::Game
{

    class PauseMenu : public Layer
    {
        using UnpauseCallbackFn  = std::function<void(void)>;
        using ExitGameCallbackFn = std::function<void(void)>;
        using SaveGameCallbackFn = std::function<void(void)>;

    public:
        PauseMenu(bool activate=true);
        virtual ~PauseMenu(void);

        void set_unpause_callback_fn(const UnpauseCallbackFn& fn);
        void set_exit_game_callback_fn(const UnpauseCallbackFn& fn);
        void set_save_game_callback_fn(const SaveGameCallbackFn& fn);

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_update(Timestep timestep) override;
        virtual void on_render(void) override;
        virtual void on_event(Event& event) override;

    private:
        virtual bool on_key_pressed(const KeyPressedEvent& event) override;

        void open_settings(void);
        void save_game(void);
        void unpause(void);
        void exit_game(void);
        void quit_to_menu(void);

    private:
        const std::unordered_map<const char*, std::function<void(void)>> m_options;
        Ref<SettingsMenu> m_settingsMenu;

        UnpauseCallbackFn  m_onUnpause;
        ExitGameCallbackFn m_onExitGame;
        SaveGameCallbackFn m_onSaveGame;
    };
}

#endif // DRK_GAME_LAYERS_PAUSE_MENU_HPP
