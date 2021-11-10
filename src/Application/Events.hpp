#ifndef DRK_EVENTS_HPP
#define DRK_EVENTS_HPP

#include "Core/Base.hpp"
#include "Event.hpp"
#include "Input.hpp"

#define DRK_EVENT_TYPE_INFO(type)                                            \
    static constexpr EventType static_type(void) { return EventType::type; } \
    EventType   get_type(void) const override { return static_type(); }      \
    const char* get_name(void) const override { return #type"Event";  }

#define DRK_EVENT_CATEGORY_INFO(cat)                \
    EventCategory get_category(void) const override \
        { return EventCategory::cat; }

namespace DrkCraft
{
    ////////// Window Events //////////

    struct WindowEvent : Event
    {
        DRK_EVENT_CATEGORY_INFO(Window);
    };

    struct WindowCloseEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowClose);

        WindowCloseEvent(void) = default;
    };

    struct WindowResizeEvent : WindowEvent
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

    struct WindowMovedEvent : WindowEvent
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

    struct WindowFocusGainedEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowFocusGained);

        WindowFocusGainedEvent(void) = default;
    };

    struct WindowFocusLostEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowFocusLost);

        WindowFocusLostEvent(void) = default;
    };

    struct WindowMinimizedEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowMinimized);

        WindowMinimizedEvent(void) = default;
    };

    struct WindowRestoredEvent : WindowEvent
    {
        DRK_EVENT_TYPE_INFO(WindowRestored);

        WindowRestoredEvent(void) = default;
    };

    ////////// Key Events //////////

    struct KeyEvent : Event
    {
        DRK_EVENT_CATEGORY_INFO(Key);

        KeyEvent(KeyCode key, InputModFlags mods)
          : key(key),
            mods(mods)
        { }

        operator std::string(void) const override;

        const KeyCode key;
        const InputModFlags mods;
    };

    struct KeyPressedEvent : KeyEvent
    {
        DRK_EVENT_TYPE_INFO(KeyPressed);

        KeyPressedEvent(KeyCode key, InputModFlags mods)
          : KeyEvent(key, mods)
        { }
    };

    struct KeyHeldEvent : KeyEvent
    {
        DRK_EVENT_TYPE_INFO(KeyHeld);

        KeyHeldEvent(KeyCode key, InputModFlags mods)
          : KeyEvent(key, mods)
        { }
    };

    struct KeyReleasedEvent : KeyEvent
    {
        DRK_EVENT_TYPE_INFO(KeyReleased);

        KeyReleasedEvent(KeyCode key, InputModFlags mods)
          : KeyEvent(key, mods)
        { }
    };

    struct CharTypedEvent : Event
    {
        DRK_EVENT_TYPE_INFO(CharTyped);
        DRK_EVENT_CATEGORY_INFO(Char);

        CharTypedEvent(char ch)
          : ch(ch)
        { }

        operator std::string(void) const override;

        const char ch;
    };

    ////////// Mouse Events //////////

    struct MousePosEvent : Event
    {
        MousePosEvent(float xPos, float yPos)
          : xPos(xPos),
            yPos(yPos)
        { }

        virtual operator std::string(void) const override;

        const float xPos;
        const float yPos;
    };

    struct MouseMovedEvent : MousePosEvent
    {
        DRK_EVENT_TYPE_INFO(MouseMoved);
        DRK_EVENT_CATEGORY_INFO(Mouse);

        MouseMovedEvent(float xPos, float yPos)
          : MousePosEvent(xPos, yPos)
        { }
    };

    struct MouseButtonEvent : MousePosEvent
    {
        DRK_EVENT_CATEGORY_INFO(MouseButton);

        MouseButtonEvent(MouseCode button, InputModFlags mods)
          : MousePosEvent(get_mouse_x(), get_mouse_y()),
            button(button),
            mods(mods)
        { }

        operator std::string(void) const override;

        const MouseCode button;
        const InputModFlags mods;
    };

    struct MouseButtonPressedEvent : MouseButtonEvent
    {
        DRK_EVENT_TYPE_INFO(MouseButtonPressed);

        MouseButtonPressedEvent(MouseCode button, InputModFlags mods)
          : MouseButtonEvent(button, mods)
        { }
    };

    struct MouseButtonReleasedEvent : MouseButtonEvent
    {
        DRK_EVENT_TYPE_INFO(MouseButtonReleased);

        MouseButtonReleasedEvent(MouseCode button, InputModFlags mods)
          : MouseButtonEvent(button, mods)
        { }
    };

    struct ScrollWheelMovedEvent : Event
    {
        DRK_EVENT_TYPE_INFO(ScrollWheelMoved);
        DRK_EVENT_CATEGORY_INFO(Mouse);

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
#undef DRK_EVENT_CATEGORY_INFO

#endif // DRK_EVENTS_HPP
