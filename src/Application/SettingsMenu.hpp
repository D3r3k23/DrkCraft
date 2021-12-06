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
        virtual ~SettingsMenu(void);

        void set_close_callback_fn(const SettingsMenuCloseCallbackFn& fn);

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_update(Timestep timestep) override;
        virtual void on_render(void) override;
        virtual void on_event(Event& event) override;

    private:
        virtual bool on_key_pressed(const KeyPressedEvent& event) override;

        void apply(void);
        void save(void);
        void close(void);

    private:
        Ptr<SettingsData> m_settings;
        bool m_dirty;

        SettingsMenuCloseCallbackFn m_onClose;
    };
}

#endif // DRK_SETTINGS_MENU_HPP
