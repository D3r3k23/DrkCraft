#ifndef DRK_HUD_LAYER_HPP
#define DRK_HUD_LAYER_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Timestep.hpp"
#include "Application/Events.hpp"

#include <functional>
#include <concepts>

namespace DrkCraft
{
    class HudLayer : public Layer
    {
    public:
        HudLayer(void);
        ~HudLayer(void);

        void on_attach(void) override;
        void on_detach(void) override;

        void on_update(Timestep timestep) override;
        void on_render(Timestep timestep) override;
        void on_event(Event& event) override;

    private:

    private:

    };
}

#endif // DRK_HUD_LAYER_HPP
