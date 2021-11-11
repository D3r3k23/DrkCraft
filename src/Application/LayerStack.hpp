#ifndef DRK_LAYER_STACK_HPP
#define DRK_LAYER_STACK_HPP

#include "Core/Base.hpp"
#include "Layer.hpp"

#include <deque>

namespace DrkCraft
{
    using LayerDeque = std::deque<Ref<Layer>>;

    class LayerStack
    {
    public:
        LayerStack(void) = default;
        LayerStack(const LayerStack& other);
        ~LayerStack(void);

        void clear(void);

        void push_front(const Ref<Layer>& layer);
        void push_back(const Ref<Layer>& layer);

        bool pop(const Ref<Layer>& layer);

        void refresh(void); // Pops detached Layers

        // Activates the front layer
        // Should only be used if all layers are inactive
        void activate_front(void);

        bool is_empty(void) const;
        bool has_active_layer(void) const;

    private:
        LayerDeque m_layers;

    public:
        // Note: Modifying a LayerStack will invalidate any of its Views
        //   In Application, we first copy the LayerStack and create the Views
        //   using the copy. Since the layers are stored in refence-counted
        //   pointers, they will remain in existance until the LayerStack copy
        //   is destroyed.

        class ForwardView
        {
        public:
            ForwardView(const LayerStack& layerStack)
              : m_layerStack(layerStack)
            { }

            LayerDeque::const_iterator begin(void) const
            {
                return m_layerStack.m_layers.begin();
            }

            LayerDeque::const_iterator end(void) const
            {
                return m_layerStack.m_layers.end();
            }

        private:
            const LayerStack& m_layerStack;
        };

        class ReverseView
        {
        public:
            ReverseView(const LayerStack& layerStack)
              : m_layerStack(layerStack)
            { }

            LayerDeque::const_reverse_iterator begin(void) const
            {
                return m_layerStack.m_layers.rbegin();
            }

            LayerDeque::const_reverse_iterator end(void) const
            {
                return m_layerStack.m_layers.rend();
            }

        private:
            const LayerStack& m_layerStack;
        };
    };
}

#endif // DRK_LAYER_STACK_HPP
