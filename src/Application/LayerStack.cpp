#include "LayerStack.hpp"

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

    Ref<Layer> LayerStack::pop_front(void)
    {
        if (!is_empty())
        {
            const auto& layer = m_layers.front();
            DRK_LOG_TRACE("Popping Layer (front): {}", layer->get_layer_name());
            m_layers.pop_front();
            return layer;
        }
        else
            return {};
    }

    Ref<Layer> LayerStack::pop_back(void)
    {
        if (!is_empty())
        {
            const auto& layer = m_layers.back();
            DRK_LOG_TRACE("Popping Layer (back): {}", layer->get_layer_name());
            m_layers.pop_back();
            return layer;
        }
        else
            return {};
    }

    bool LayerStack::remove(const Ref<Layer>& layer)
    {
        if (auto it = std::find(m_layers.begin(), m_layers.end(), layer); it != m_layers.end())
        {
            DRK_LOG_TRACE("Removing Layer: {}", layer->get_layer_name());
            m_layers.erase(it);
            return true;
        }
        else
            return false;
    }

    void LayerStack::refresh(void)
    {
        std::vector<Ref<Layer>> toRemove;
        for (const auto& layer : m_layers)
            if (layer->is_layer_detached())
                toRemove.push_back(layer);

        for (const auto& layer : toRemove)
            remove(layer);
    }

    void LayerStack::activate_front(void)
    {
        DRK_ASSERT(!is_empty(), "LayerStack is empty");
        auto& layer = m_layers.front();
        DRK_ASSERT(!(layer->is_layer_active()), "Front layer already active");
        layer->activate_layer();
    }

    bool LayerStack::is_empty(void) const
    {
        return m_layers.empty();
    }

    bool LayerStack::has_active_layer(void) const
    {
        return std::any_of(m_layers.begin(), m_layers.end(), [](const auto& layer)
        {
            return layer->is_layer_active();
        });
    }
}
