#ifndef DRK_PAUSE_MENU_HPP
#define DRK_PAUSE_MENU_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Timestep.hpp"
#include "Application/Events.hpp"

#include <functional>

namespace DrkCraft
{
    using PauseMenuUnpauseCallbackFn  = std::function<void(void)>;
    using PauseMenuExitGameCallbackFn = std::function<void(void)>;
    using PauseMenuSaveGameCallbackFn = std::function<void(void)>;

    class PauseMenu : public Layer
    {
    public:
        PauseMenu(void);
        ~PauseMenu(void);

        void set_unpause_callback_fn(const PauseMenuUnpauseCallbackFn& fn);
        void set_exit_game_callback_fn(const PauseMenuUnpauseCallbackFn& fn);
        void set_save_game_callback_fn(const PauseMenuSaveGameCallbackFn& fn);

        void on_attach(void) override;
        void on_detach(void) override;

        void on_update(Timestep timestep) override;
        void on_render(Timestep timestep) override;
        void on_event(Event& event) override;

    private:
        bool on_key_pressed(KeyPressedEvent& event);
        bool on_mouse_button_pressed(MouseButtonPressedEvent& event);

    private:
        PauseMenuUnpauseCallbackFn  m_onUnpause;
        PauseMenuExitGameCallbackFn m_onExitGame;
        PauseMenuSaveGameCallbackFn m_onSaveGame;
    };
}

#endif // DRK_PAUSE_MENU_HPP
