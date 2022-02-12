#ifndef DRK_GAME_LAYERS_CONSOLE_HPP
#define DRK_GAME_LAYERS_CONSOLE_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Events.hpp"
#include "Application/Timestep.hpp"

namespace DrkCraft
{
    class Console : public Layer
    {
    public:
        Console(bool activate=false);
        virtual ~Console(void);

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_update(Timestep timestep) override;
        virtual void on_render(void) override;
        virtual void on_event(Event& event) override;

    private:

    private:

    };
}

#endif // DRK_GAME_LAYERS_CONSOLE_HPP
