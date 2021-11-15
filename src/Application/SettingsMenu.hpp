#ifndef DRK_SETTINGS_MENU_HPP
#define DRK_SETTINGS_MENU_HPP

#include "Core/Base.hpp"
#include "Layer.hpp"
#include "Timestep.hpp"
#include "Events.hpp"
#include "Core/RunSettings.hpp"

#include <functional>

namespace DrkCraft
{
    using SettingsMenuCloseCallbackFn = std::function<void(void)>;

    class SettingsMenu : public Layer
    {
    public:
        SettingsMenu(bool activate=true);
        ~SettingsMenu(void);

        void set_close_callback_fn(const SettingsMenuCloseCallbackFn& fn);

        void on_attach(void) override;
        void on_detach(void) override;

        void on_update(Timestep timestep) override;
        void on_render(Timestep timestep) override;
        void on_event(Event& event) override;

    private:
        bool on_mouse_button_pressed(MouseButtonPressedEvent& event);

    private:
        Ptr<SettingsData> m_settings;

        SettingsMenuCloseCallbackFn m_onClose;
    };
}

#endif // DRK_SETTINGS_MENU_HPP
