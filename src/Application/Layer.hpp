#ifndef DRK_LAYER_HPP
#define DRK_LAYER_HPP

#include "Core/Base.hpp"
#include "Events.hpp"
#include "Core/Timestep.hpp"

#include <string>
#include <string_view>
#include <utility>

namespace DrkCraft
{
    class Layer;

    template <typename L>
    concept ConcreteLayerConcept = std::derived_from<L, Layer> && !std::same_as<L, Layer>;

    // Layer/~Layer: Actual construction/destruction of the Layer, use to init/clean up resources
    // on_attach/on_detach (optional): Called when Layer is pushed/popped from the LayerStack
    // on_update/on_render/on_event: Core logic
    // activate/deactivate: Inactive Layers are ignored when copying LayerStack
    // detach_layer: Marks a Layer to be removed from the LayerStack

    // Layers should be attached using Application::add_overlay/add_layer
    // Layers can remove themselves using detach_layer

    // Weird things might happen if we detach/reattach or deactivate/activate a Layer
    // in the same frame

    class Layer
    {
    public:
        template<ConcreteLayerConcept L, typename ... Args>
        constexpr static Ref<L> create(Args&& ... args)
        {
            return make_ptr<L>(std::forward<Args>(args)...);
        }

        Layer(std::string_view name, bool activate=true);
        virtual ~Layer(void);

        virtual void on_attach(void) { }
        virtual void on_detach(void) { }

        virtual void on_update(Timestep timestep) = 0;
        virtual void on_render(void) = 0;
        virtual void on_event(Event& e) = 0;

        void attach_layer(void);
        void detach_layer(void);

        void activate_layer(void);
        void deactivate_layer(void);

        bool is_layer_active(void) const;
        bool is_layer_attached(void) const;
        bool is_layer_detached(void) const;

        std::string_view get_layer_name(void) const;

    private:
        const std::string m_layerName;

        bool m_layerActive;
        bool m_layerAttached;

    private:
        // Window Event handlers
        virtual bool on_window_event(const WindowEvent& event) { return false; }
        virtual bool on_window_closed(const WindowClosedEvent& event) { return false; }
        virtual bool on_window_resized(const WindowResizedEvent& event) { return false; }
        virtual bool on_framebuffer_resized(const FramebufferResizedEvent& event) { return false; }
        virtual bool on_window_moved(const WindowMovedEvent& event) { return false; }
        virtual bool on_window_focus_gained(const WindowFocusGainedEvent& event) { return false; }
        virtual bool on_window_focus_lost(const WindowFocusLostEvent& event) { return false; }
        virtual bool on_window_maximized(const WindowMaximizedEvent& event) { return false; }
        virtual bool on_window_minimized(const WindowMinimizedEvent& event) { return false; }
        virtual bool on_window_restored(const WindowRestoredEvent& event) { return false; }
        virtual bool on_window_scaled(const WindowScaledEvent& event) { return false; }
        virtual bool on_window_refreshed(const WindowRefreshedEvent& event) { return false; }

        // Input Event handlers
        virtual bool on_input_event(const InputEvent& event) { return false; }

        // Keyboard Event handlers
        virtual bool on_keyboard_event(const KeyboardEvent& event) { return false; }
        virtual bool on_key_event(const KeyEvent& event) { return false; }
        virtual bool on_key_pressed(const KeyPressedEvent& event) { return false; }
        virtual bool on_key_held(const KeyHeldEvent& event) { return false; }
        virtual bool on_key_released(const KeyReleasedEvent& event) { return false; }
        virtual bool on_char_typed(const CharTypedEvent& event) { return false; }

        // Mouse Event handlers
        virtual bool on_mouse_event(const MouseEvent& event) { return false; }
        virtual bool on_mouse_pos_event(const MousePosEvent& event) { return false; }
        virtual bool on_mouse_button_event(const MouseButtonEvent& event) { return false; }
        virtual bool on_mouse_button_pressed(const MouseButtonPressedEvent& event) { return false; }
        virtual bool on_mouse_button_released(const MouseButtonReleasedEvent& event) { return false; }
        virtual bool on_mouse_moved(const MouseMovedEvent& event) { return false; }
        virtual bool on_scroll_wheel_moved(const ScrollWheelMovedEvent& event) { return false; }

        // Monitor Event handlers
        virtual bool on_monitor_event(const MonitorEvent& event) { return false; }
        virtual bool on_monitor_connected(const MonitorConnectedEvent& event) { return false; }
        virtual bool on_monitor_disconnected(const MonitorDisconnectedEvent& event) { return false; }
    };
}

#endif // DRK_LAYER_HPP
