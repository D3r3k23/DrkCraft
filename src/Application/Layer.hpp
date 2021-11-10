#ifndef DRK_LAYER_HPP
#define DRK_LAYER_HPP

#include "Core/Base.hpp"
#include "Events.hpp"
#include "Timestep.hpp"

#include <string>
#include <string_view>

namespace DrkCraft
{
    class Layer
    {
    public:
        Layer(std::string_view name="Layer")
          : m_layerName(name),
            m_layerActive(true),
            m_layerDeleted(false)
        {
            DRK_LOG_TRACE("Creating Layer: ", m_layerName);
        }

        virtual ~Layer(void) = default;

        virtual void on_update(Timestep timestep) = 0;
        virtual void on_render(Timestep timestep) = 0;
        virtual void on_event(Event& e) = 0;

        void close_layer(void)
        {
            m_layerDeleted = true;
        }

        std::string get_name(void) const
        {
            return m_layerName;
        }

        bool is_active(void) const
        {
            return m_layerActive && !m_layerDeleted;
        }

        bool is_deleted(void) const
        {
            return m_layerDeleted;
        }

    protected:
        const std::string m_layerName;
        bool m_layerActive;
        bool m_layerDeleted;
    };
}

#endif // DRK_LAYER_HPP
