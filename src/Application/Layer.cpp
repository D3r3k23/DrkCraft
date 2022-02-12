#include "Layer.hpp"

#include "Core/Debug/Profiler.hpp"

namespace DrkCraft
{
    Layer::Layer(std::string_view name, bool activate)
      : m_layerName(name),
        m_layerActive(false),
        m_layerAttached(false)
    {
        DRK_LOG_CORE_TRACE("Creating Layer: {}", get_layer_name());
        if (activate)
            activate_layer();
    }

    Layer::~Layer(void)
    {
        DRK_LOG_CORE_TRACE("Deleted Layer: {}", get_layer_name());
    }

    void Layer::attach_layer(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_INFO("Attaching Layer: {}", get_layer_name());

        m_layerAttached = true;
        on_attach();
    }

    void Layer::detach_layer(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_INFO("Detaching Layer: {}", get_layer_name());

        m_layerAttached = false;
        on_detach();
    }

    void Layer::activate_layer(void)
    {
        DRK_LOG_CORE_INFO("Activating Layer: {}", get_layer_name());
        m_layerActive = true;
    }

    void Layer::deactivate_layer(void)
    {
        DRK_LOG_CORE_INFO("Deactivating Layer: {}", get_layer_name());
        m_layerActive = false;
    }

    bool Layer::is_layer_active(void) const
    {
        return m_layerActive && m_layerAttached;
    }

    bool Layer::is_layer_attached(void) const
    {
        return m_layerAttached;
    }

    bool Layer::is_layer_detached(void) const
    {
        return !m_layerAttached;
    }

    std::string_view Layer::get_layer_name(void) const
    {
        return m_layerName;
    }
}
