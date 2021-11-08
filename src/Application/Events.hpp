#ifndef DRK_EVENTS_HPP
#define DRK_EVENTS_HPP

#include "Event.hpp"

#define DRK_EVENT_TYPE_INFO(type)                                            \
    static constexpr EventType static_type(void) { return EventType::type; } \
    EventType   get_type(void) const override { return static_type(); }      \
    const char* get_name(void) const override { return #type"Event";  }

namespace DrkCraft
{
    ////////// Window Events //////////

    struct WindowCloseEvent : Event
    {
        DRK_EVENT_TYPE_INFO(WindowClose);

        WindowCloseEvent(void) = default;
    };

    struct WindowResizeEvent : Event
    {
        DRK_EVENT_TYPE_INFO(WindowResize);

        WindowResizeEvent(int width, int height)
          : width(width),
            height(height)
        { }

        operator std::string(void) const override;

        const uint width;
        const uint height;
    };

    struct WindowMovedEvent : Event
    {
        DRK_EVENT_TYPE_INFO(WindowMoved);

        WindowMovedEvent(int xPos, int yPos)
          : xPos(xPos),
            yPos(yPos)
        { }

        operator std::string(void) const override;

        const uint xPos;
        const uint yPos;
    };

    struct WindowFocusedEvent : Event
    {
        DRK_EVENT_TYPE_INFO(WindowFocused);

        WindowFocusedEvent(void) = default;
    };

    struct WindowFocusLostEvent : Event
    {
        DRK_EVENT_TYPE_INFO(WindowFocusLost);

        WindowFocusLostEvent(void) = default;
    };

    struct WindowMinimizedEvent : Event
    {
        DRK_EVENT_TYPE_INFO(WindowMinimized);

        WindowMinimizedEvent(void) = default;
    };

    struct WindowRestoredEvent : Event
    {
        DRK_EVENT_TYPE_INFO(WindowRestored);

        WindowRestoredEvent(void) = default;
    };

    ////////// Key Events //////////

    struct KeyEvent : Event
    {
        KeyEvent(int key)
          : key(key_code_from_glfw(key))
        { }

        operator std::string(void) const override;

        const KeyCode key;
    };

    struct KeyPressedEvent : KeyEvent
    {
        DRK_EVENT_TYPE_INFO(KeyPressed);

        KeyPressedEvent(int key)
          : KeyEvent(key)
        { }
    };

    struct KeyHeldEvent : KeyEvent
    {
        DRK_EVENT_TYPE_INFO(KeyHeld);

        KeyHeldEvent(int key)
          : KeyEvent(key)
        { }
    };

    struct KeyReleasedEvent : KeyEvent
    {
        DRK_EVENT_TYPE_INFO(KeyReleased);

        KeyReleasedEvent(int key)
          : KeyEvent(key)
        { }
    };

    struct CharTypedEvent : Event
    {
        DRK_EVENT_TYPE_INFO(CharTyped);

        CharTypedEvent(uint codePoint)
          : ch((char)codePoint)
        { }

        operator std::string(void) const override;

        const char ch;
    };

    ////////// Mouse Events //////////

    struct MouseButtonEvent : Event
    {
        MouseButtonEvent(int button)
          : button(mouse_code_from_glfw(button))
        { }

        operator std::string(void) const override;

        const MouseCode button;
    };

    struct MouseButtonPressedEvent : MouseButtonEvent
    {
        DRK_EVENT_TYPE_INFO(MouseButtonPressed);

        MouseButtonPressedEvent(int button)
          : MouseButtonEvent(button)
        { }
    };

    struct MouseButtonReleasedEvent : MouseButtonEvent
    {
        DRK_EVENT_TYPE_INFO(MouseButtonReleased);

        MouseButtonReleasedEvent(int button)
          : MouseButtonEvent(button)
        { }
    };

    struct MouseMovedEvent : Event
    {
        DRK_EVENT_TYPE_INFO(MouseMoved);

        MouseMovedEvent(double xPos, double yPos)
          : xPos((float)xPos),
            yPos((float)yPos)
        { }

        operator std::string(void) const override;

        const float xPos;
        const float yPos;
    };

    struct MouseScrolledEvent : Event
    {
        DRK_EVENT_TYPE_INFO(MouseScrolled);

        MouseScrolledEvent(double xOffset, double yOffset)
          : xOffset((float)xOffset),
            yOffset((float)yOffset)
        { }

        operator std::string(void) const override;

        const float xOffset;
        const float yOffset;
    };
}

#undef DRK_EVENT_TYPE_INFO

#endif // DRK_EVENTS_HPP
