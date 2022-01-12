#ifndef DRK_EVENTS_HPP
#define DRK_EVENTS_HPP

#include "Core/Base.hpp"
#include "Event.hpp"
#include "System/Input.hpp"

#define DRK_EVENT_TYPE_INFO(flagType, name)            \
    virtual const char* get_name(void) const override   \
        { return #name"Event"; }                         \
    virtual EventFlags get_type(void) const override      \
        { return static_cast<EventFlags>(static_type()); } \
    static constexpr flagType static_type(void)             \
        { return flagType::name; }

namespace DrkCraft
{
    ////////// Window Events //////////

    struct WindowEvent : Event
    {
        DRK_EVENT_TYPE_INFO(EventCategory, Window)
    };

    struct WindowClosedEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, WindowClosed)

        WindowClosedEvent(void) = default;
    };

    struct WindowResizedEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, WindowResized)

        WindowResizedEvent(uint width, uint height)
          : width(width),
            height(height)
        { }

        virtual std::string get_details(void) const override;

        const uint width;
        const uint height;
    };

    struct FramebufferResizedEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, FramebufferResized)

        FramebufferResizedEvent(uint width, uint height)
          : width(width),
            height(height)
        { }

        virtual std::string get_details(void) const override;

        const uint width;
        const uint height;
    };

    struct WindowMovedEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, WindowMoved)

        WindowMovedEvent(int xPos, int yPos)
          : xPos(xPos),
            yPos(yPos)
        { }

        virtual std::string get_details(void) const override;

        const int xPos;
        const int yPos;
    };

    struct WindowFocusGainedEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, WindowFocusGained)

        WindowFocusGainedEvent(void) = default;
    };

    struct WindowFocusLostEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, WindowFocusLost)

        WindowFocusLostEvent(void) = default;
    };

    struct WindowMaximizedEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, WindowMaximized)

        WindowMaximizedEvent(void) = default;
    };

    struct WindowMinimizedEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, WindowMinimized)

        WindowMinimizedEvent(void) = default;
    };

    // Could add param to say if it was restored from maximized of minimized
    struct WindowRestoredEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, WindowRestored)

        WindowRestoredEvent(void) = default;
    };

    struct WindowScaledEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, WindowScaled)

        WindowScaledEvent(float xScale, float yScale)
          : xScale(xScale),
            yScale(yScale)
        { }

        virtual std::string get_details(void) const override;

        float xScale;
        float yScale;
    };

    struct WindowRefreshedEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, WindowRefreshed)

        WindowRefreshedEvent(void) = default;
    };

    ////////// Input Events //////////

    struct InputEvent : Event
    {
        DRK_EVENT_TYPE_INFO(EventCategory, Input);
    };

    ////////// Keyboard Events //////////

    struct KeyboardEvent : InputEvent
    {
        DRK_EVENT_TYPE_INFO(EventCategory, Keyboard)
    };

    struct KeyEvent : KeyboardEvent
    {
        DRK_EVENT_TYPE_INFO(EventCategory, Keyboard)

        KeyEvent(KeyCode key, KeyModFlags mods)
          : key(key),
            mods(mods)
        { }

        virtual std::string get_details(void) const override;

        const KeyCode key;
        const KeyModFlags mods;
    };

    struct KeyPressedEvent : KeyEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, KeyPressed)

        KeyPressedEvent(KeyCode key, KeyModFlags mods)
          : KeyEvent(key, mods)
        { }
    };

    struct KeyHeldEvent : KeyEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, KeyHeld)

        KeyHeldEvent(KeyCode key, KeyModFlags mods)
          : KeyEvent(key, mods)
        { }
    };

    struct KeyReleasedEvent : KeyEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, KeyReleased)

        KeyReleasedEvent(KeyCode key, KeyModFlags mods)
          : KeyEvent(key, mods)
        { }
    };

    struct CharTypedEvent : KeyboardEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, CharTyped)

        CharTypedEvent(char ch)
          : ch(ch)
        { }

        virtual std::string get_details(void) const override;

        const char ch;
    };

    ////////// Mouse Events //////////

    struct MouseEvent : InputEvent
    {
        DRK_EVENT_TYPE_INFO(EventCategory, Mouse)
    };

    struct MousePosEvent : MouseEvent
    {
        DRK_EVENT_TYPE_INFO(EventCategory, MousePos)

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
        DRK_EVENT_TYPE_INFO(EventType, MouseMoved)

        MouseMovedEvent(float xPos, float yPos)
          : MousePosEvent(xPos, yPos)
        { }
    };

    struct MouseButtonEvent : MousePosEvent
    {
        DRK_EVENT_TYPE_INFO(EventCategory, MouseButton)

        MouseButtonEvent(MouseCode button, KeyModFlags mods)
          : MousePosEvent(get_mouse_x(), get_mouse_y()),
            button(button),
            mods(mods)
        { }

        virtual std::string get_details(void) const override;

        const MouseCode button;
        const KeyModFlags mods;
    };

    struct MouseButtonPressedEvent : MouseButtonEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, MouseButtonPressed)

        MouseButtonPressedEvent(MouseCode button, KeyModFlags mods)
          : MouseButtonEvent(button, mods)
        { }
    };

    struct MouseButtonReleasedEvent : MouseButtonEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, MouseButtonReleased)

        MouseButtonReleasedEvent(MouseCode button, KeyModFlags mods)
          : MouseButtonEvent(button, mods)
        { }
    };

    struct ScrollWheelMovedEvent : MouseEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, ScrollWheelMoved)

        ScrollWheelMovedEvent(float xOffset, float yOffset)
          : xOffset(xOffset),
            yOffset(yOffset)
        { }

        virtual std::string get_details(void) const override;

        const float xOffset;
        const float yOffset;
    };

    ////////// Monitor Events //////////

    struct MonitorEvent : Event
    {
        DRK_EVENT_TYPE_INFO(EventCategory, Monitor)

        MonitorEvent(uint number)
          : number(number)
        { }

        virtual std::string get_details(void) const override;

        const uint number;
    };

    struct MonitorConnectedEvent : MonitorEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, MonitorConnected)

        MonitorConnectedEvent(uint number)
          : MonitorEvent(number)
        { }
    };

    struct MonitorDisconnectedEvent : MonitorEvent
    {
        DRK_EVENT_TYPE_INFO(EventType, MonitorDisconnected)

        MonitorDisconnectedEvent(uint number)
          : MonitorEvent(number)
        { }
    };
}

#undef DRK_EVENT_TYPE_INFO

#endif // DRK_EVENTS_HPP
