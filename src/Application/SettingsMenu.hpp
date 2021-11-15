#ifndef DRK_SETTINGS_MENU_HPP
#define DRK_SETTINGS_MENU_HPP

#include "Core/Base.hpp"
#include "Layer.hpp"
#include "Timestep.hpp"
#include "Events.hpp"

namespace DrkCraft
{
    class SettingsMenu : public Layer
    {
    public:
        SettingsMenu(void);
        ~SettingsMenu(void);

        void on_attach(void) override;
        void on_detach(void) override;

        void on_update(Timestep timestep) override;
        void on_render(Timestep timestep) override;
        void on_event(Event& event) override;

    private:

    };
}

#endif // DRK_SETTINGS_MENU_HPP
