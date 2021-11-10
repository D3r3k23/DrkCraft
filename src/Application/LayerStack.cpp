#include "LayerStack.hpp"

namespace DrkCraft
{
    LayerStack::~LayerStack(void)
    {
        for (Layer* layer : m_layers)
        {
            delete layer;
        }
    }

    void LayerStack::push_front(Layer* layer)
    {
        DRK_LOG_TRACE("Pushing layer (front): {}", layer->get_name());
        m_layers.push_front(layer);
    }

    void LayerStack::push_back(Layer* layer)
    {
        DRK_LOG_TRACE("Pushing layer (back): {}", layer->get_name());
        m_layers.push_back(layer);
    }

    void LayerStack::pop_front(void)
    {
        Layer* layer = m_layers.front();
        DRK_LOG_TRACE("Popping layer (front): {}", layer->get_name());
        m_layers.pop_front();
        delete layer;
    }

    void LayerStack::pop_back(void)
    {
        Layer* layer = m_layers.back();
        DRK_LOG_TRACE("Popping layer (back): {}", layer->get_name());
        m_layers.pop_back();
        delete layer;
    }

    bool LayerStack::remove(Layer* layer)
    {
        if (auto it = std::find(m_layers.begin(), m_layers.end(), layer); it != m_layers.end())
        {
            DRK_LOG_TRACE("Removing layer: {}", layer->get_name());
            m_layers.erase(it);
            delete layer;
            return true;
        }
        else
            return false;
    }

    bool LayerStack::is_empty(void) const
    {
        return m_layers.empty();
    }
}
