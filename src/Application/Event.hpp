#ifndef DRK_EVENT_HPP
#define DRK_EVENT_HPP

// Probably should not include directly, use Application/Events.hpp instead

#include "Core/Base.hpp"

#include <string>
#include <functional>
#include <utility>
#include <concepts>

#define DRK_BIND_EVENT_HANDLER(fn) [this](auto&& ... args) -> bool \
{                                                                  \
    return this->fn(std::forward<decltype(args)>(args)...);        \
}

namespace DrkCraft
{
    enum class EventType
    {
        None = 0,

        // Window Events
        WindowClose,
        WindowResize, // Framebuffer size
        WindowMoved,
        WindowFocused,
        WindowFocusLost,
        WindowMinimized, // Maximized
        WindowRestored,  // Restored from maximized/minimzed?

        // Key Events
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

        Window      = 0b000001,
        Input       = 0b000010,
        Key         = 0b000110,
        Char        = 0b001010,
        Mouse       = 0b010010,
        MouseButton = 0b110010
    };

    EventCategoryFlags operator|(EventCategoryFlags flags, EventCategory cat);
    EventCategoryFlags operator|(EventCategory cat, EventCategoryFlags flags);
    EventCategoryFlags operator|(EventCategory cat1, EventCategory cat2);

    bool operator==(EventCategory cat, EventCategoryFlags flags);
    bool operator==(EventCategoryFlags flags, EventCategory cat);

    bool operator!=(EventCategory cat, EventCategoryFlags flags);
    bool operator!=(EventCategoryFlags flags, EventCategory cat);

    struct Event
    {
        bool handled = false;

        ~Event(void) = default;

        virtual EventType   get_type(void) const = 0;
        virtual const char* get_name(void) const = 0;

        virtual EventCategory get_category(void) const = 0;

        virtual operator std::string(void) const;
    };

    bool operator==(const Event& event, EventCategoryFlags flags);
    bool operator==(EventCategoryFlags flags, const Event& event);

    bool operator!=(const Event& event, EventCategoryFlags flags);
    bool operator!=(EventCategoryFlags flags, const Event& event);

    void log_event(const Event& event);

    template <typename E>
    concept EventConcept = std::derived_from<E, Event>;

    template <EventConcept E>
    using EventHandlerFn = std::function<bool(E&)>;

    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event)
          : event(event)
        { }

        // Calls event handler function for events of type E
        // Handler returns true if event handled
        template <EventConcept E>
        void dispatch(const EventHandlerFn<E>& handler)
        {
            if (!event.handled && event.get_type() == E::static_type())
                event.handled = handler(static_cast<E&>(event));
        }

    private:
        Event& event;
    };
}

#endif // DRK_EVENT_HPP
