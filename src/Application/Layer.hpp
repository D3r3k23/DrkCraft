#ifndef DRK_LAYER_HPP
#define DRK_LAYER_HPP

#include "Core/Base.hpp"
#include "Events.hpp"
#include "Timestep.hpp"

#include <string>
#include <string_view>
#include <utility>

namespace DrkCraft
{
    class Layer;

    template <typename L>
    concept ConcreteLayerConcept = std::derived_from<L, Layer> && !std::same_as<L, Layer>;

    // Layer/~Layer: Actual construction/destruction of the Layer, use to init/clean up resources
    // on_attach/on_detach (optional): Called when Layer is pushed/popped from the LayerStack
    // on_update/on_render/on_event: Core logic
    // activate/deactivate: Inactive Layers are ignored when copying LayerStack

    class Layer
    {
    public:
        template<ConcreteLayerConcept L, typename ... Args>
        constexpr static Ref<L> create(Args&& ... args)
        {
            return make_ptr<L>(std::forward<Args>(args)...);
        }

        Layer(std::string_view name, bool activate=true);
        virtual ~Layer(void);

        virtual void on_attach(void) { }
        virtual void on_detach(void) { }

        virtual void on_update(Timestep timestep) = 0;
        virtual void on_render(Timestep timestep) = 0;
        virtual void on_event(Event& e) = 0;

        void attach_layer(void);
        void detach_layer(void);

        void activate_layer(void);
        void deactivate_layer(void);

        bool is_layer_active(void) const;
        bool is_layer_attached(void) const;
        bool is_layer_detached(void) const;

        std::string get_layer_name(void) const;

    private:
        const std::string m_layerName;

        bool m_layerActive;
        bool m_layerAttached;
    };
}

#endif // DRK_LAYER_HPP
