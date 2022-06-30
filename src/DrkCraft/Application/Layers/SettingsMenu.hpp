#ifndef DRK_APPLICATION_LAYERS_SETTINGS_MENU_HPP
#define DRK_APPLICATION_LAYERS_SETTINGS_MENU_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Events.hpp"
#include "Util/Timestep.hpp"

#include "Core/Settings.hpp"

#include <magic_enum.hpp>

#include <vector>
#include <functional>

namespace DrkCraft
{
    enum class SettingModificationState
    {
        Clean = 0,
        Dirty
    };

    class SettingsMenu : public Layer
    {
        using CloseCallbackFn = std::function<void(void)>;

    public:
        SettingsMenu(bool activate=true);
        virtual ~SettingsMenu(void);

        void set_close_callback_fn(const CloseCallbackFn& fn);

        void open(void);

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_render(void) override;
        virtual void on_update(Timestep timestep) override;
        virtual void on_event(Event& event) override;

    private:
        virtual bool on_key_pressed(const KeyPressedEvent& event) override;

        void save(void);
        void apply(void);
        void close(void);

        void load_settings(void);

        void make_dirty(Setting setting);
        bool dirty(Setting setting) const;
        bool keybinds_dirty(void) const;

    private:
        SettingsData m_settings;
        std::vector<SettingModificationState> m_dirty;

        KeyBinds m_keybinds;
        SettingModificationState m_keybindsDirty;

        CloseCallbackFn m_onClose;
    };
}

#endif // DRK_APPLICATION_LAYERS_SETTINGS_MENU_HPP
