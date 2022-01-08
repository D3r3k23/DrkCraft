#include "LayerStack.hpp"

#include "Core/Profiler.hpp"

#include <ranges>

namespace DrkCraft
{
    LayerStack LayerStack::copy_active(const LayerStack& src)
    {
        LayerStack copy;
        std::ranges::copy_if(src.m_layers, std::back_inserter(copy.m_layers), [](const auto& layer)
        {
            return layer->is_layer_active();
        });
        return copy;
    }

    void LayerStack::push_front(const Ref<Layer>& layer)
    {
        DRK_LOG_CORE_TRACE("Pushing Layer (front): {}", layer->get_layer_name());
        m_layers.push_front(layer);
    }

    void LayerStack::push_back(const Ref<Layer>& layer)
    {
        DRK_LOG_CORE_TRACE("Pushing Layer (back): {}", layer->get_layer_name());
        m_layers.push_back(layer);
    }

    bool LayerStack::pop(const Ref<Layer>& layer)
    {
        if (auto it = std::ranges::find(m_layers, layer); it != m_layers.end())
        {
            DRK_LOG_CORE_TRACE("Popping Layer: {}", layer->get_layer_name());
            m_layers.erase(it);
            return true;
        }
        else
            return false;
    }

    void LayerStack::refresh(void)
    {
        DRK_PROFILE_FUNCTION();

        std::vector<Ref<Layer>> detachedLayers;
        for (const auto& layer : m_layers)
            if (layer->is_layer_detached() && !layer->is_layer_active())
                detachedLayers.push_back(layer);

        for (const auto& layer : detachedLayers)
            pop(layer);
    }

    void LayerStack::clear(void)
    {
        m_layers.clear();
    }

    void LayerStack::activate_back(void)
    {
        DRK_ASSERT_DEBUG(!empty(), "LayerStack is empty");
        auto& layer = m_layers.back();
        DRK_ASSERT_DEBUG(!layer->is_layer_active(), "Back layer already active");
        layer->activate_layer();
    }

    bool LayerStack::empty(void) const
    {
        return m_layers.empty();
    }

    bool LayerStack::all_layers_inactive(void) const
    {
        return std::ranges::all_of(m_layers, [](const auto& layer)
        {
            return !layer->is_layer_active();
        });
    }

    LayerStack::ForwardView::ForwardView(const LayerStack& layerStack)
      : m_layerStack(layerStack)
    { }

    LayerDeque::const_iterator LayerStack::ForwardView::begin(void) const
    {
        return m_layerStack.m_layers.begin();
    }

    LayerDeque::const_iterator LayerStack::ForwardView::end(void) const
    {
        return m_layerStack.m_layers.end();
    }

    LayerStack::ReverseView::ReverseView(const LayerStack& layerStack)
      : m_layerStack(layerStack)
    { }

    LayerDeque::const_reverse_iterator LayerStack::ReverseView::begin(void) const
    {
        return m_layerStack.m_layers.rbegin();
    }

    LayerDeque::const_reverse_iterator LayerStack::ReverseView::end(void) const
    {
        return m_layerStack.m_layers.rend();
    }
}
