#ifndef DRK_APPLICATION_LAYERS_LOADING_SCREEN_HPP
#define DRK_APPLICATION_LAYERS_LOADING_SCREEN_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Events.hpp"
#include "Util/Timestep.hpp"

#include "Util/PeriodicUpdate.hpp"

#include <string>
#include <string_view>

namespace DrkCraft
{
    // Set message / progress
    class LoadingScreen : public Layer
    {
    public:
        LoadingScreen(std::string_view msg="Loading", bool activate=true);
        virtual ~LoadingScreen(void);

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_render(void) override;
        virtual void on_update(Timestep timestep) override;
        virtual void on_event(Event& event) override;

        void set_message(std::string_view msg);

    private:
        IntervalTimer m_updateTimer;
        std::string m_msg;
        uint m_numPeriods;
    };
}

#endif // DRK_APPLICATION_LAYERS_LOADING_SCREEN_HPP
