#ifndef DRK_APPLICATION_LAYERS_LOADING_SCREEN_HPP
#define DRK_APPLICATION_LAYERS_LOADING_SCREEN_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Events.hpp"
#include "Application/Timestep.hpp"

namespace DrkCraft
{
    class LoadingScreen : public Layer
    {
    public:
        LoadingScreen(bool activate=true);
        virtual ~LoadingScreen(void);

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_update(Timestep timestep) override;
        virtual void on_render(void) override;
        virtual void on_event(Event& event) override;

    private:
        IntervalTimer m_updateTimer;
        uint m_numPeriods;
    };
}

#endif // DRK_APPLICATION_LAYERS_LOADING_SCREEN_HPP
