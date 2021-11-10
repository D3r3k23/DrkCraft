#ifndef DRK_LAYER_STACK_HPP
#define DRK_LAYER_STACK_HPP

#include "Core/Base.hpp"
#include "Layer.hpp"

#include <deque>
#include <concepts>

namespace DrkCraft
{
    using LayerDeque = std::deque<Layer*>;

    template <typename I>
    concept LayerDequeIteratorConcept = std::convertible_to<I, LayerDeque::const_iterator>
                                     || std::convertible_to<I, LayerDeque::const_reverse_iterator>;;

    class LayerStack
    {
    public:
        LayerStack(void) = default;
        ~LayerStack(void);

        void push_front(Layer* layer);
        void push_back(Layer* layer);

        void pop_front(void);
        void pop_back(void);

        bool remove(Layer* layer);

        bool is_empty(void) const;

    private:
        LayerDeque m_layers;

    public:
        class View
        {
        public:
            View(LayerStack& layerStack)
              : m_layerStack(layerStack)
            { }

            LayerDeque::iterator begin(void)
            {
                return m_layerStack.m_layers.begin();
            }

            LayerDeque::iterator end(void)
            {
                return m_layerStack.m_layers.end();
            }

            LayerDeque::const_iterator begin(void) const
            {
                return m_layerStack.m_layers.begin();
            }

            LayerDeque::const_iterator end(void) const
            {
                return m_layerStack.m_layers.end();
            }

        private:
            LayerStack& m_layerStack;
        };

        class ReverseView
        {
        public:
            ReverseView(LayerStack& layerStack)
              : m_layerStack(layerStack)
            { }

            LayerDeque::reverse_iterator begin(void)
            {
                return m_layerStack.m_layers.rbegin();
            }

            LayerDeque::reverse_iterator end(void)
            {
                return m_layerStack.m_layers.rend();
            }

            LayerDeque::const_reverse_iterator begin(void) const
            {
                return m_layerStack.m_layers.rbegin();
            }

            LayerDeque::const_reverse_iterator end(void) const
            {
                return m_layerStack.m_layers.rend();
            }

        private:
            LayerStack& m_layerStack;
        };
    };
}

#endif // DRK_LAYER_STACK_HPP
