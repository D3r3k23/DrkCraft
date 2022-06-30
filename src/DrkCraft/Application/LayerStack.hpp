#ifndef DRK_APPLICATION_LAYER_STACK_HPP
#define DRK_APPLICATION_LAYER_STACK_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"

#include <deque>

namespace DrkCraft
{
    using LayerDeque = std::deque<Ref<Layer>>;

    using LayerIterator = LayerDeque::const_iterator;
    using ReverseLayerIterator = LayerDeque::const_reverse_iterator;

    class LayerStack
    {
    public:
        LayerStack(void);
        ~LayerStack(void);

        static LayerStack copy_active(const LayerStack& src);

        // Should only move LayerStacks, copying only needs to preserve active layers
        LayerStack(LayerStack&&) = default;
        LayerStack(const LayerStack&) = delete;
        LayerStack& operator=(LayerStack&&) = default;
        LayerStack& operator=(const LayerStack&) = delete;

        void push(const Ref<Layer>& layer, bool overlay=false);
        Error<> pop(const Ref<Layer>& layer);

        void refresh(void); // Pops detached Layers
        void clear(void);

        // Activates the back layer
        // Should only need to be used if all layers are inactive
        void activate_back(void);

        bool empty(void) const;
        bool all_layers_inactive(void) const;

    private:
        LayerDeque m_layers;
        uint m_nOverlays;

    public:
        // Note: Modifying a LayerStack will invalidate any of its Views
        //   In Application, we first copy the LayerStack using LayerStack::copy_active
        //   and create the Views using the copy. Since the layers are stored in
        //   refence-counted pointers, they will remain in existance until the LayerStack
        //   copy is destroyed.

        class ForwardView
        {
        public:
            ForwardView(const LayerStack& layerStack) : m_layerStack(layerStack) {}

            LayerIterator begin(void) const { return m_layerStack.m_layers.cbegin(); }
            LayerIterator end(void)  const  { return m_layerStack.m_layers.cend();  }

        private:
            const LayerStack& m_layerStack;
        };

        class ReverseView
        {
        public:
            ReverseView(const LayerStack& layerStack) : m_layerStack(layerStack) {}

            ReverseLayerIterator begin(void) const { return m_layerStack.m_layers.crbegin(); }
            ReverseLayerIterator end(void)  const  { return m_layerStack.m_layers.crend();  }

        private:
            const LayerStack& m_layerStack;
        };
    };
}

#endif // DRK_APPLICATION_LAYER_STACK_HPP
