#ifndef DRK_PAUSE_MENU_HPP
#define DRK_PAUSE_MENU_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Timestep.hpp"
#include "Application/Events.hpp"

#include <functional>

namespace DrkCraft
{
    using PauseMenuSaveGameCallbackFn = std::function<void(void)>;
    using PauseMenuUnpauseCallbackFn  = std::function<void(bool)>; // bool unpaused

    class PauseMenu : public Layer
    {
    public:
        PauseMenu(void);
        ~PauseMenu(void);

        void set_save_game_callback_fn(const PauseMenuSaveGameCallbackFn& fn);
        void set_unpause_callback_fn(const PauseMenuUnpauseCallbackFn& fn);

        void on_attach(void) override;
        void on_detach(void) override;

        void on_update(Timestep timestep) override;
        void on_render(Timestep timestep) override;
        void on_event(Event& event) override;

    private:
        bool on_key_pressed(KeyPressedEvent& event);
        bool on_mouse_button_pressed(MouseButtonPressedEvent& event);

    private:
        PauseMenuSaveGameCallbackFn m_onSaveGame;
        PauseMenuUnpauseCallbackFn  m_onUnpause;
    };
}

#endif // DRK_PAUSE_MENU_HPP
