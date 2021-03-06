#include "LayerStack.hpp"

#include "Core/Debug/Profiler.hpp"

#include <algorithm>

namespace DrkCraft
{
    LayerStack::LayerStack(void)
      : m_nOverlays(0)
    { }

    LayerStack::~LayerStack(void)
    {
        while (!m_layers.empty())
        {
            auto it = m_layers.end() - 1;
            m_layers.erase(it);
        }
    }

    LayerStack LayerStack::copy_active(const LayerStack& src)
    {
        DRK_PROFILE_FUNCTION();

        LayerStack copy;
        copy.m_nOverlays = src.m_nOverlays;

        std::ranges::copy_if(src.m_layers, std::back_inserter(copy.m_layers), [](const auto& layer)
        {
            return layer->is_layer_active();
        });
        return copy;
    }

    void LayerStack::push(const Ref<Layer>& layer, bool overlay)
    {
        DRK_PROFILE_FUNCTION();

        if (overlay)
        {
            DRK_LOG_CORE_TRACE("Pushing overlay Layer: {}", layer->get_layer_name());
            m_layers.push_front(layer);
            ++m_nOverlays;
        }
        else
        {
            DRK_LOG_CORE_TRACE("Pushing Layer: {}", layer->get_layer_name());
            const auto pos = m_layers.begin() + m_nOverlays;
            m_layers.insert(pos, layer);
        }
    }

    Error<> LayerStack::pop(const Ref<Layer>& layer)
    {
        DRK_PROFILE_FUNCTION();

        if (auto it = std::ranges::find(m_layers, layer); it != m_layers.end())
        {
            if (it - m_layers.begin() < m_nOverlays)
            {
                DRK_LOG_CORE_TRACE("Popping overlay Layer: {}", layer->get_layer_name());
                --m_nOverlays;
            }
            else
                DRK_LOG_CORE_TRACE("Popping Layer: {}", layer->get_layer_name());

            m_layers.erase(it);
            return Error<>::Success;
        }
        else
            return Error<>::Failure;
    }

    void LayerStack::refresh(void)
    {
        DRK_PROFILE_FUNCTION();

        std::deque<Ref<Layer>> detachedLayers;
        for (const auto& layer : m_layers)
            if (layer->is_layer_detached() && !layer->is_layer_active())
                detachedLayers.push_front(layer);

        for (const auto& layer : detachedLayers)
            pop(layer);
    }

    void LayerStack::clear(void)
    {
        DRK_PROFILE_FUNCTION();
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
}
