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

    struct Event
    {
        ~Event(void) = default;

        virtual EventType   get_type(void) const = 0;
        virtual const char* get_name(void) const = 0;

        virtual operator std::string(void) const;

        bool handled = false;
    };

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
