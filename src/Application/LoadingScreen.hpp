#ifndef DRK_LOADING_SCREEN_HPP
#define DRK_LOADING_SCREEN_HPP

#include "Core/Base.hpp"
#include "Layer.hpp"
#include "Events.hpp"
#include "Core/Timestep.hpp"
#include "Core/Util.hpp"

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

#endif // DRK_LOADING_SCREEN_HPP
