#ifndef DRK_EVENTS_HPP
#define DRK_EVENTS_HPP

#include "Core/Base.hpp"
#include "Event.hpp"
#include "Input.hpp"

#define DRK_EVENT_TYPE_INFO(type)                                             \
    static constexpr EventType static_type(void) { return EventType::type; } \
    EventType   get_type(void) const override { return static_type(); }     \
    const char* get_name(void) const override { return #type##"Event";  }  \
    DRK_PROFILE_OBJECT(#type##"Event");

#define DRK_EVENT_CATEGORY_INFO(cat)                     \
    EventCategoryFlags get_category(void) const override  \
        { return static_cast<EventCategoryFlags>(EventCategory::cat); }

namespace DrkCraft
{
    ////////// Window Events //////////

    struct WindowEvent : Event
    {
        DRK_EVENT_CATEGORY_INFO(Window)
    };

    struct WindowClosedEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowClosed)

        WindowClosedEvent(void) = default;
    };

    struct WindowResizedEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowResized)

        WindowResizedEvent(uint width, uint height)
          : width(width),
            height(height)
        { }

        std::string get_details(void) const override;

        const uint width;
        const uint height;
    };

    struct FramebufferResizedEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(FramebufferResized)

        FramebufferResizedEvent(uint width, uint height)
          : width(width),
            height(height)
        { }

        std::string get_details(void) const override;

        const uint width;
        const uint height;
    };

    struct WindowMovedEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowMoved)

        WindowMovedEvent(int xPos, int yPos)
          : xPos(xPos),
            yPos(yPos)
        { }

        std::string get_details(void) const override;

        const int xPos;
        const int yPos;
    };

    struct WindowFocusGainedEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowFocusGained)

        WindowFocusGainedEvent(void) = default;
    };

    struct WindowFocusLostEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowFocusLost)

        WindowFocusLostEvent(void) = default;
    };

    struct WindowMaximizedEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowMaximized)

        WindowMaximizedEvent(void) = default;
    };

    struct WindowMinimizedEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowMinimized)

        WindowMinimizedEvent(void) = default;
    };

    struct WindowRestoredEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowRestored)

        WindowRestoredEvent(void) = default;
    };

    struct WindowScaledEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowScaled)

        WindowScaledEvent(float xScale, float yScale)
          : xScale(xScale),
            yScale(yScale)
        { }

        std::string get_details(void) const override;

        float xScale;
        float yScale;
    };

    struct WindowRefreshedEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowRefreshed)

        WindowRefreshedEvent(void) = default;
    };

    ////////// Keyboard Events //////////

    struct KeyEvent : Event
    {
        DRK_EVENT_CATEGORY_INFO(Keyboard)

        KeyEvent(KeyCode key, InputModFlags mods)
          : key(key),
            mods(mods)
        { }

        virtual std::string get_details(void) const override;

        const KeyCode key;
        const InputModFlags mods;
    };

    struct KeyPressedEvent : KeyEvent
    {
        DRK_EVENT_TYPE_INFO(KeyPressed)

        KeyPressedEvent(KeyCode key, InputModFlags mods)
          : KeyEvent(key, mods)
        { }
    };

    struct KeyHeldEvent : KeyEvent
    {
        DRK_EVENT_TYPE_INFO(KeyHeld)

        KeyHeldEvent(KeyCode key, InputModFlags mods)
          : KeyEvent(key, mods)
        { }
    };

    struct KeyReleasedEvent : KeyEvent
    {
        DRK_EVENT_TYPE_INFO(KeyReleased)

        KeyReleasedEvent(KeyCode key, InputModFlags mods)
          : KeyEvent(key, mods)
        { }
    };

    struct CharTypedEvent : Event
    {
        DRK_EVENT_TYPE_INFO(CharTyped)
        DRK_EVENT_CATEGORY_INFO(Keyboard)

        CharTypedEvent(char ch)
          : ch(ch)
        { }

        std::string get_details(void) const override;

        const char ch;
    };

    ////////// Mouse Events //////////

    struct MousePosEvent : Event
    {
        MousePosEvent(float xPos, float yPos)
          : xPos(xPos),
            yPos(yPos)
        { }

        virtual std::string get_details(void) const override;

        const float xPos;
        const float yPos;
    };

    struct MouseMovedEvent : MousePosEvent
    {
        DRK_EVENT_TYPE_INFO(MouseMoved)
        DRK_EVENT_CATEGORY_INFO(Mouse)

        MouseMovedEvent(float xPos, float yPos)
          : MousePosEvent(xPos, yPos)
        { }
    };

    struct MouseButtonEvent : MousePosEvent
    {
        DRK_EVENT_CATEGORY_INFO(MouseButton)

        MouseButtonEvent(MouseCode button, InputModFlags mods)
          : MousePosEvent(get_mouse_x(), get_mouse_y()),
            button(button),
            mods(mods)
        { }

        virtual std::string get_details(void) const override;

        const MouseCode button;
        const InputModFlags mods;
    };

    struct MouseButtonPressedEvent : MouseButtonEvent
    {
        DRK_EVENT_TYPE_INFO(MouseButtonPressed)

        MouseButtonPressedEvent(MouseCode button, InputModFlags mods)
          : MouseButtonEvent(button, mods)
        { }
    };

    struct MouseButtonReleasedEvent : MouseButtonEvent
    {
        DRK_EVENT_TYPE_INFO(MouseButtonReleased)

        MouseButtonReleasedEvent(MouseCode button, InputModFlags mods)
          : MouseButtonEvent(button, mods)
        { }
    };

    struct ScrollWheelMovedEvent : Event
    {
        DRK_EVENT_TYPE_INFO(ScrollWheelMoved)
        DRK_EVENT_CATEGORY_INFO(Mouse)

        ScrollWheelMovedEvent(float xOffset, float yOffset)
          : xOffset(xOffset),
            yOffset(yOffset)
        { }

        std::string get_details(void) const override;

        const float xOffset;
        const float yOffset;
    };

    // Monitor Events

    // Callbacks for these are not yet implemented
    struct MonitorEvent : Event
    {
        DRK_EVENT_CATEGORY_INFO(Monitor)
    };

    struct MonitorConnectedEvent : MonitorEvent
    {
        DRK_EVENT_TYPE_INFO(MonitorConnected)

        MonitorConnectedEvent(void) = default;
    };

    struct MonitorDisconnectedEvent : MonitorEvent
    {
        DRK_EVENT_TYPE_INFO(MonitorDisconnected)

        MonitorDisconnectedEvent(void) = default;
    };
}

#undef DRK_EVENT_TYPE_INFO
#undef DRK_EVENT_CATEGORY_INFO

#endif // DRK_EVENTS_HPP
