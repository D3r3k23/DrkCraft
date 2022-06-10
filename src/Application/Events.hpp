#ifndef DRK_APPLICATION_EVENTS_HPP
#define DRK_APPLICATION_EVENTS_HPP

#include "Core/Base.hpp"
#include "Application/Event.hpp"
#include "System/Input.hpp"

#define DRK_EVENT_TYPE_INFO(name)                                             \
    virtual const char* get_name(void) const override { return #name"Event"; } \
    virtual EventType   get_type(void) const override { return static_type(); } \
    static constexpr EventType static_type(void)      { return EventType::name; }

#define DRK_EVENT_CATEGORY_INFO(name) \
    static constexpr EventCategory static_type(void) { return EventCategory::name; }

namespace DrkCraft
{
    ////////// Window Events //////////

    struct WindowEvent : Event
    {
        DRK_EVENT_CATEGORY_INFO(Window)
    };

    struct WindowClosedEvent final : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowClosed)

        WindowClosedEvent(void) = default;
    };

    struct WindowResizedEvent final : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowResized)

        WindowResizedEvent(uint width, uint height)
          : width(width),
            height(height)
        { }

        virtual std::string get_details(void) const override;

        const uint width;
        const uint height;
    };

    struct FramebufferResizedEvent final : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(FramebufferResized)

        FramebufferResizedEvent(uint width, uint height)
          : width(width),
            height(height)
        { }

        virtual std::string get_details(void) const override;

        const uint width;
        const uint height;
    };

    struct WindowMovedEvent final : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowMoved)

        WindowMovedEvent(int xPos, int yPos)
          : xPos(xPos),
            yPos(yPos)
        { }

        virtual std::string get_details(void) const override;

        const int xPos;
        const int yPos;
    };

    struct WindowFocusGainedEvent final : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowFocusGained)

        WindowFocusGainedEvent(void) = default;
    };

    struct WindowFocusLostEvent final : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowFocusLost)

        WindowFocusLostEvent(void) = default;
    };

    struct WindowMaximizedEvent final : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowMaximized)

        WindowMaximizedEvent(void) = default;
    };

    struct WindowMinimizedEvent final : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowMinimized)

        WindowMinimizedEvent(void) = default;
    };

    struct WindowRestoredEvent final : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowRestored)

        enum Source
        {
            FromMaximized,
            FromMinimized
        };

        WindowRestoredEvent(Source source)
          : source(source)
        { }

        virtual std::string get_details(void) const override;

        const Source source;
    };

    struct WindowScaledEvent final : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowScaled)

        WindowScaledEvent(float xScale, float yScale)
          : xScale(xScale),
            yScale(yScale)
        { }

        virtual std::string get_details(void) const override;

        const float xScale;
        const float yScale;
    };

    struct WindowRefreshedEvent final : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowRefreshed)

        WindowRefreshedEvent(void) = default;
    };

    ////////// Input Events //////////

    struct InputEvent : Event
    {
        DRK_EVENT_CATEGORY_INFO(Input);

        virtual InputCode get_input_code(void) const;
    };

    ////////// Keyboard Events //////////

    struct KeyboardEvent : InputEvent
    {
        DRK_EVENT_CATEGORY_INFO(Keyboard)
    };

    struct KeyEvent : KeyboardEvent
    {
        DRK_EVENT_CATEGORY_INFO(Keyboard)

        KeyEvent(KeyCode key, KeyModFlags mods)
          : key(key),
            mods(mods)
        { }

        virtual std::string get_details(void) const override;
        virtual InputCode get_input_code(void) const override;

        const KeyCode key;
        const KeyModFlags mods;
    };

    struct KeyPressedEvent final : KeyEvent
    {
        DRK_EVENT_TYPE_INFO(KeyPressed)

        KeyPressedEvent(KeyCode key, KeyModFlags mods)
          : KeyEvent(key, mods)
        { }
    };

    struct KeyHeldEvent final : KeyEvent
    {
        DRK_EVENT_TYPE_INFO(KeyHeld)

        KeyHeldEvent(KeyCode key, KeyModFlags mods)
          : KeyEvent(key, mods)
        { }
    };

    struct KeyReleasedEvent final : KeyEvent
    {
        DRK_EVENT_TYPE_INFO(KeyReleased)

        KeyReleasedEvent(KeyCode key, KeyModFlags mods)
          : KeyEvent(key, mods)
        { }
    };

    struct CharTypedEvent final : KeyboardEvent
    {
        DRK_EVENT_TYPE_INFO(CharTyped)

        CharTypedEvent(char ch)
          : ch(ch)
        { }

        virtual std::string get_details(void) const override;

        const char ch;
    };

    ////////// Mouse Events //////////

    struct MouseEvent : InputEvent
    {
        DRK_EVENT_CATEGORY_INFO(Mouse)
    };

    struct MousePosEvent : MouseEvent
    {
        DRK_EVENT_CATEGORY_INFO(MousePos)

        MousePosEvent(float xPos, float yPos)
          : xPos(xPos),
            yPos(yPos)
        { }

        virtual std::string get_details(void) const override;

        const float xPos;
        const float yPos;
    };

    struct MouseMovedEvent final : MousePosEvent
    {
        DRK_EVENT_TYPE_INFO(MouseMoved)

        MouseMovedEvent(float xPos, float yPos)
          : MousePosEvent(xPos, yPos)
        { }
    };

    struct MouseButtonEvent : MousePosEvent
    {
        DRK_EVENT_CATEGORY_INFO(MouseButton)

        MouseButtonEvent(MouseCode button, KeyModFlags mods)
          : MousePosEvent(get_mouse_x(), get_mouse_y()),
            button(button),
            mods(mods)
        { }

        virtual std::string get_details(void) const override;
        virtual InputCode get_input_code(void) const override;

        const MouseCode button;
        const KeyModFlags mods;
    };

    struct MouseButtonPressedEvent final : MouseButtonEvent
    {
        DRK_EVENT_TYPE_INFO(MouseButtonPressed)

        MouseButtonPressedEvent(MouseCode button, KeyModFlags mods)
          : MouseButtonEvent(button, mods)
        { }
    };

    struct MouseButtonReleasedEvent final : MouseButtonEvent
    {
        DRK_EVENT_TYPE_INFO(MouseButtonReleased)

        MouseButtonReleasedEvent(MouseCode button, KeyModFlags mods)
          : MouseButtonEvent(button, mods)
        { }
    };

    struct ScrollWheelMovedEvent final : MouseEvent
    {
        DRK_EVENT_TYPE_INFO(ScrollWheelMoved)

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
        DRK_EVENT_CATEGORY_INFO(Monitor)

        MonitorEvent(uint number)
          : number(number)
        { }

        virtual std::string get_details(void) const override;

        const uint number;
    };

    struct MonitorConnectedEvent final : MonitorEvent
    {
        DRK_EVENT_TYPE_INFO(MonitorConnected)

        MonitorConnectedEvent(uint number)
          : MonitorEvent(number)
        { }
    };

    struct MonitorDisconnectedEvent final : MonitorEvent
    {
        DRK_EVENT_TYPE_INFO(MonitorDisconnected)

        MonitorDisconnectedEvent(uint number)
          : MonitorEvent(number)
        { }
    };
}

#undef DRK_EVENT_TYPE_INFO

#endif // DRK_APPLICATION_EVENTS_HPP
