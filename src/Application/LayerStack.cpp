#include "LayerStack.hpp"

#include "Core/Profiler.hpp"

#include <algorithm>

namespace DrkCraft
{
    LayerStack::LayerStack(const LayerStack& other)
      : m_layers(other.m_layers)
    { }

    LayerStack::~LayerStack(void)
    {
        clear();
    }

    void LayerStack::clear(void)
    {
        m_layers.clear();
    }

    void LayerStack::push_front(const Ref<Layer>& layer)
    {
        DRK_LOG_TRACE("Pushing Layer (front): {}", layer->get_layer_name());
        m_layers.push_front(layer);
    }

    void LayerStack::push_back(const Ref<Layer>& layer)
    {
        DRK_LOG_TRACE("Pushing Layer (back): {}", layer->get_layer_name());
        m_layers.push_back(layer);
    }

    bool LayerStack::pop(const Ref<Layer>& layer)
    {
        if (auto it = std::find(m_layers.begin(), m_layers.end(), layer); it != m_layers.end())
        {
            DRK_LOG_TRACE("Popping Layer: {}", layer->get_layer_name());
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

    void LayerStack::activate_back(void)
    {
        DRK_ASSERT(!is_empty(), "LayerStack is empty");
        auto& layer = m_layers.back();
        DRK_ASSERT(!layer->is_layer_active(), "Back layer already active");
        layer->activate_layer();
    }

    bool LayerStack::is_empty(void) const
    {
        return m_layers.empty();
    }

    bool LayerStack::all_layers_inactive(void) const
    {
        return std::all_of(m_layers.begin(), m_layers.end(), [](const auto& layer)
        {
            return !layer->is_layer_active();
        });
    }
}
