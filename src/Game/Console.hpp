#ifndef DRK_CONSOLE_HPP
#define DRK_CONSOLE_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Events.hpp"
#include "Core/Timestep.hpp"

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

#endif // DRK_CONSOLE_HPP
