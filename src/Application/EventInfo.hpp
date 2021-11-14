#ifndef DRK_EVENT_INFO_HPP
#define DRK_EVENT_INFO_HPP

// Do not include directly, use Application/Events.hpp instead

#include "Core/Base.hpp"

namespace DrkCraft
{
    class Event;

    enum class EventType
    {
        None = 0,

        // Window Events
        WindowClose,
        WindowResize,
        FramebufferResize,
        WindowMoved,
        WindowFocusGained,
        WindowFocusLost,
        WindowMinimized, // Maximized
        WindowRestored,  // Restored from maximized/minimzed?

        // Keyboard Events
        KeyPressed,
        KeyHeld,
        KeyReleased,
        CharTyped,

        // Mouse Events
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        ScrollWheelMoved
    };

    using EventCategoryFlags = uint;

    enum class EventCategory : EventCategoryFlags
    {
        None = 0,

        Window      = 0b00001,
        Input       = 0b00010,
        Keyboard    = 0b00110,
        Mouse       = 0b01010,
        MouseButton = 0b11010
    };

    EventCategoryFlags operator|(EventCategoryFlags flags, EventCategory cat);
    EventCategoryFlags operator|(EventCategory cat, EventCategoryFlags flags);
    EventCategoryFlags operator|(EventCategory cat1, EventCategory cat2);

    bool operator==(EventCategory cat, EventCategoryFlags flags);
    bool operator==(EventCategoryFlags flags, EventCategory cat);

    bool operator!=(EventCategory cat, EventCategoryFlags flags);
    bool operator!=(EventCategoryFlags flags, EventCategory cat);

    bool operator==(const Event& event, EventCategoryFlags flags);
    bool operator==(EventCategoryFlags flags, const Event& event);

    bool operator!=(const Event& event, EventCategoryFlags flags);
    bool operator!=(EventCategoryFlags flags, const Event& event);

    bool operator==(const Event& event, EventCategory cat);
    bool operator==(EventCategory cat, const Event& event);

    bool operator!=(const Event& event, EventCategory cat);
    bool operator!=(EventCategory cat, const Event& event);
}

#endif // DRK_EVENT_INFO_HPP
