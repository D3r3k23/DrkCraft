#include "SettingsMenu.hpp"

#include "Core/Profiler.hpp"

#include <imgui/imgui.h>

namespace DrkCraft
{
    SettingsMenu::SettingsMenu(bool activate)
      : Layer("SettingsMenuLayer", activate)
    { }

    SettingsMenu::~SettingsMenu(void)
    {

    }

    void SettingsMenu::set_close_callback_fn(const SettingsMenuCloseCallbackFn& fn)
    {
        m_onClose = fn;
    }

    void SettingsMenu::on_attach(void)
    {
        DRK_PROFILE_FUNCTION();

        m_settings = make_ptr<SettingsData>(RuntimeSettings::get());
    }

    void SettingsMenu::on_detach(void)
    {

    }

    void SettingsMenu::on_update(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();


    }

    void SettingsMenu::on_render(Timestep timestep)
    {
        DRK_PROFILE_FUNCTION();


    }

    void SettingsMenu::on_event(Event& event)
    {
        DRK_PROFILE_FUNCTION();

        EventDispatcher ed(event);
        ed.dispatch<MouseButtonPressedEvent>(DRK_BIND_FN(on_mouse_button_pressed));
    }

    bool SettingsMenu::on_mouse_button_pressed(MouseButtonPressedEvent& event)
    {
        if (event.button == MouseCode::Left)
        {
            RuntimeSettings::set(*m_settings);
            RuntimeSettings::save_to_file();
            return true;
        }
        else if (event.button == MouseCode::Right)
        {
            m_onClose();
            deactivate_layer();
            return true;
        }
        else
            return false;
    }
}
