#ifndef DRK_APPLICATION_LAYERS_SETTINGS_MENU_HPP
#define DRK_APPLICATION_LAYERS_SETTINGS_MENU_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Events.hpp"
#include "Application/Timestep.hpp"

#include "Core/RunSettings.hpp"

#include <array>
#include <functional>

namespace DrkCraft
{
    class SettingsMenu : public Layer
    {
        using CloseCallbackFn = std::function<void(void)>;

    public:
        SettingsMenu(bool activate=true);
        virtual ~SettingsMenu(void);

        void set_close_callback_fn(const CloseCallbackFn& fn);

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_update(Timestep timestep) override;
        virtual void on_render(void) override;
        virtual void on_event(Event& event) override;

    private:
        virtual bool on_key_pressed(const KeyPressedEvent& event) override;

        void save(void);
        void apply(void);
        void close(void);

    private:
        SettingsData m_settings;
        std::array<bool, NUM_SETTINGS> m_dirty;

        CloseCallbackFn m_onClose;
    };
}

#endif // DRK_APPLICATION_LAYERS_SETTINGS_MENU_HPP
