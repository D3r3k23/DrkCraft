#ifndef LAYER_HPP
#define LAYER_HPP

#include "Events.hpp"
#include "Core/Base.hpp"
#include "Core/Timestep.hpp"

#include <string_view>

namespace DrkCraft
{
    class Layer
    {
    public:
        Layer(std::string_view name = "Layer");
        virtual ~Layer(void) = default;

        virtual void on_attach(void) = 0;
        virtual void on_detach(void) = 0;

        virtual void on_update(Timestep ts) = 0;
        virtual void on_event(Event& e) = 0;

    private:
    };
}

#endif // LAYER_HPP
