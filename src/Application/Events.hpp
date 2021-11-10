#ifndef DRK_EVENTS_HPP
#define DRK_EVENTS_HPP

#include "Core/Base.hpp"
#include "Event.hpp"
#include "Input.hpp"

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

        WindowResizeEvent(uint width, uint height)
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

        WindowMovedEvent(uint xPos, uint yPos)
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
        KeyEvent(KeyCode key, InputMod mods)
          : key(key),
            mods(mods)
        { }

        operator std::string(void) const override;

        const KeyCode key;
        const InputMod mods;
    };

    struct KeyPressedEvent : KeyEvent
    {
        DRK_EVENT_TYPE_INFO(KeyPressed);

        KeyPressedEvent(KeyCode key, InputMod mods)
          : KeyEvent(key, mods)
        { }
    };

    struct KeyHeldEvent : KeyEvent
    {
        DRK_EVENT_TYPE_INFO(KeyHeld);

        KeyHeldEvent(KeyCode key, InputMod mods)
          : KeyEvent(key, mods)
        { }
    };

    struct KeyReleasedEvent : KeyEvent
    {
        DRK_EVENT_TYPE_INFO(KeyReleased);

        KeyReleasedEvent(KeyCode key, InputMod mods)
          : KeyEvent(key, mods)
        { }
    };

    struct CharTypedEvent : Event
    {
        DRK_EVENT_TYPE_INFO(CharTyped);

        CharTypedEvent(char ch)
          : ch(ch)
        { }

        operator std::string(void) const override;

        const char ch;
    };

    ////////// Mouse Events //////////

    struct MouseEvent : Event
    {
        MouseEvent(float xPos, float yPos)
          : xPos(xPos),
            yPos(yPos)
        { }

        operator std::string(void) const override;

        const float xPos;
        const float yPos;
    };

    struct MouseMovedEvent : MouseEvent
    {
        DRK_EVENT_TYPE_INFO(MouseMoved);

        MouseMovedEvent(float xPos, float yPos)
          : MouseEvent(xPos, yPos)
        { }
    };

    struct MouseButtonEvent : MouseEvent
    {
        MouseButtonEvent(MouseCode button, InputMod mods)
          : MouseEvent(get_mouse_x(), get_mouse_y()),
            button(button),
            mods(mods)
        { }

        operator std::string(void) const override;

        const MouseCode button;
        const InputMod mods;
    };

    struct MouseButtonPressedEvent : MouseButtonEvent
    {
        DRK_EVENT_TYPE_INFO(MouseButtonPressed);

        MouseButtonPressedEvent(MouseCode button, InputMod mods)
          : MouseButtonEvent(button, mods)
        { }
    };

    struct MouseButtonReleasedEvent : MouseButtonEvent
    {
        DRK_EVENT_TYPE_INFO(MouseButtonReleased);

        MouseButtonReleasedEvent(MouseCode button, InputMod mods)
          : MouseButtonEvent(button, mods)
        { }
    };

    struct ScrollWheelMovedEvent : Event
    {
        DRK_EVENT_TYPE_INFO(ScrollWheelMoved);

        ScrollWheelMovedEvent(float xOffset, float yOffset)
          : xOffset(xOffset),
            yOffset(yOffset)
        { }

        operator std::string(void) const override;

        const float xOffset;
        const float yOffset;
    };
}

#undef DRK_EVENT_TYPE_INFO

#endif // DRK_EVENTS_HPP
