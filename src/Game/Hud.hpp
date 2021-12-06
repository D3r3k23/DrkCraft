#ifndef DRK_HUD_HPP
#define DRK_HUD_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Timestep.hpp"
#include "Application/Events.hpp"

#include <functional>
#include <concepts>

namespace DrkCraft
{
    class Hud : public Layer
    {
    public:
        Hud(bool activate);
        virtual ~Hud(void);

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_update(Timestep timestep) override;
        virtual void on_render(void) override;
        virtual void on_event(Event& event) override;

    private:

    private:

    };
}

#endif // DRK_HUD_HPP
