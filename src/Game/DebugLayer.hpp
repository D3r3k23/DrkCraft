#ifndef DRK_DEBUG_LAYER_HPP
#define DRK_DEBUG_LAYER_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Timestep.hpp"
#include "Application/Events.hpp"

namespace DrkCraft
{
    class DebugLayer : public Layer
    {
    public:
        DebugLayer(void);
        ~DebugLayer(void);

        void on_attach(void) override;
        void on_detach(void) override;

        void on_update(Timestep timestep) override;
        void on_render(Timestep timestep) override;
        void on_event(Event& event) override;

    private:

    private:

    };
}

#endif // DRK_DEBUG_LAYER_HPP
