#ifndef DRK_CONSOLE_HPP
#define DRK_CONSOLE_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Timestep.hpp"
#include "Application/Events.hpp"

namespace DrkCraft
{
    class Console : public Layer
    {
    public:
        Console(bool activate=false);
        ~Console(void);

        void on_attach(void) override;
        void on_detach(void) override;

        void on_update(Timestep timestep) override;
        void on_render(Timestep timestep) override;
        void on_event(Event& event) override;

    private:

    private:

    };
}

#endif // DRK_CONSOLE_HPP
