#ifndef DRK_APPLICATION_EVENT_HPP
#define DRK_APPLICATION_EVENT_HPP

// Probably should not include directly, use Application/Events.hpp instead

#include "Core/Base.hpp"
#include "Application/EventTypes.hpp"

#include "lib/string.hpp"

#include <concepts>
#include <type_traits>
#include <functional>
#include <utility>

namespace DrkCraft
{
    struct Event
    {
    public:
        Event(void);
        virtual ~Event(void) = default;

        Event(const Event&) = delete;
        Event(Event&&) = delete;
        Event& operator=(const Event&) = delete;
        Event& operator=(Event&&) = delete;

        virtual const char* get_name(void) const = 0;
        virtual EventType   get_type(void) const = 0;

        static constexpr EventCategory static_type(void) { return EventCategory::Event; }

        virtual string get_details(void) const;
        string get_string(void) const;

        void log_event(void) const;

        bool handled(void) const;
        void set_handled(void);

    private:
        bool m_handled;
    };

    template <typename E>
    concept EventConcept = std::derived_from<E, Event>;

    template <typename E>
    concept AbstractEventConcept = EventConcept<E> && !std::is_final<E>::value;

    template <typename E>
    concept CategoryEventConcept = AbstractEventConcept<E> && !std::same_as<E, Event>;

    template <typename E>
    concept ConcreteEventConcept = EventConcept<E> && std::is_final<E>::value;

    template <typename E>
    concept DispatchableEventConcept = ConcreteEventConcept<E> || CategoryEventConcept<E>;

    template <EventConcept E>
    const E& event_cast(const AbstractEventConcept auto& event)
    {
    #if defined(DRK_DEBUG_ENABLED)
        const E* e = dynamic_cast<const E*>(&event);
        DRK_ASSERT_DEBUG(e, "Invalid Event cast");
        return *e;
    #else
        return static_cast<const E&>(event);
    #endif
    }

    template <EventConcept E>
    E& event_cast(AbstractEventConcept auto& event)
    {
        return static_cast<E&>(event);
    }

    template <AbstractEventConcept E>
    using AbstractEventHandlerFn = std::function<void(E&)>;

    template <ConcreteEventConcept E>
    using ConcreteEventHandlerFn = std::function<void(E&)>;

    template <DispatchableEventConcept E>
    using DispatchableEventHandlerFn = std::function<bool(const E&)>; // Returns true if event was handled

    template <AbstractEventConcept E1>
    class EventDispatcher
    {
    public:
        EventDispatcher(E1& event)
          : event(event)
        { }

        template <DispatchableEventConcept E2> requires std::derived_from<E2, E1>
        void dispatch(const DispatchableEventHandlerFn<E2>& handler)
        {
            if (event == E2::static_type() && !event.handled())
            {
                bool handled = handler(event_cast<E2>(event));
                if (handled)
                    event.set_handled();
            }
        }

    private:
        E1& event;
    };
}

#endif // DRK_APPLICATION_EVENT_HPP
