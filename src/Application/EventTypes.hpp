#ifndef DRK_APPLICATION_EVENT_INFO_HPP
#define DRK_APPLICATION_EVENT_INFO_HPP

// Do not include directly, use Application/Events.hpp instead

#include "Core/Base.hpp"

namespace DrkCraft
{
    struct Event;

    using EventFlags = uint32;

    enum class EventType : EventFlags
    {
        None = 0,

        // Window Events
        WindowClosed       = 1 << 0,
        WindowResized      = 1 << 1,
        FramebufferResized = 1 << 2,
        WindowMoved        = 1 << 3,
        WindowFocusGained  = 1 << 4,
        WindowFocusLost    = 1 << 5,
        WindowMaximized    = 1 << 6,
        WindowMinimized    = 1 << 7,
        WindowRestored     = 1 << 8,
        WindowScaled       = 1 << 9,
        WindowRefreshed    = 1 << 10,

        // Keyboard Events
        KeyPressed  = 1 << 11,
        KeyHeld     = 1 << 12,
        KeyReleased = 1 << 13,
        CharTyped   = 1 << 14,

        // Mouse Events
        MouseButtonPressed  = 1 << 15,
        MouseButtonReleased = 1 << 16,
        MouseMoved          = 1 << 17,
        ScrollWheelMoved    = 1 << 18,

        // Monitor Events
        MonitorConnected    = 1 << 19,
        MonitorDisconnected = 1 << 20
    };

    enum class EventCategory : EventFlags
    {
        None = 0,

        Window = (EventFlags)EventType::WindowClosed
               | (EventFlags)EventType::WindowResized
               | (EventFlags)EventType::FramebufferResized
               | (EventFlags)EventType::WindowMoved
               | (EventFlags)EventType::WindowFocusGained
               | (EventFlags)EventType::WindowFocusLost
               | (EventFlags)EventType::WindowMaximized
               | (EventFlags)EventType::WindowMinimized
               | (EventFlags)EventType::WindowRestored
               | (EventFlags)EventType::WindowScaled
               | (EventFlags)EventType::WindowRefreshed,

        Key = (EventFlags)EventType::KeyPressed
            | (EventFlags)EventType::KeyHeld
            | (EventFlags)EventType::KeyReleased,

        Keyboard = (EventFlags)Key
                 | (EventFlags)EventType::CharTyped,

        MouseButton = (EventFlags)EventType::MouseButtonPressed
                    | (EventFlags)EventType::MouseButtonReleased,

        MousePos = (EventFlags)MouseButton
                 | (EventFlags)EventType::MouseMoved,

        Mouse = (EventFlags)MousePos
              | (EventFlags)EventType::ScrollWheelMoved,

        Input = (EventFlags)Keyboard
              | (EventFlags)Mouse,

        Monitor = (EventFlags)EventType::MonitorConnected
                | (EventFlags)EventType::MonitorDisconnected,

        Event = (EventFlags)Window
              | (EventFlags)Input
              | (EventFlags)Monitor
    };

    EventFlags to_event_flags(auto item);
    bool event_flags_has_event(const Event& event, EventFlags flags);

    bool operator==(const Event& event, EventFlags flags);
    bool operator==(const Event& event, EventType type);
    bool operator==(const Event& event, EventCategory cat);

    bool operator!=(const Event& event, EventFlags flags);
    bool operator!=(const Event& event, EventType type);
    bool operator!=(const Event& event, EventCategory cat);

    EventFlags operator|(EventFlags flags, EventType type);
    EventFlags operator|(EventFlags flags, EventCategory cat);

    EventFlags operator|(EventType type,    EventFlags flags);
    EventFlags operator|(EventCategory cat, EventFlags flags);

    EventFlags operator|(EventType type,    EventCategory cat);
    EventFlags operator|(EventCategory cat, EventType type);

    EventFlags operator|(EventType type1,    EventType type2);
    EventFlags operator|(EventCategory cat1, EventCategory cat2);

    EventFlags operator|=(EventFlags& flags, EventType type);
    EventFlags operator|=(EventFlags& flags, EventCategory cat);
}

#endif // DRK_APPLICATION_EVENT_INFO_HPP
