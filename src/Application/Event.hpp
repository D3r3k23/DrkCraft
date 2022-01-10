#ifndef DRK_EVENT_HPP
#define DRK_EVENT_HPP

// Probably should not include directly, use Application/Events.hpp instead

#include "Core/Base.hpp"
#include "EventTypes.hpp"

#include <string>
#include <functional>
#include <utility>
#include <concepts>

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
        virtual EventFlags  get_type(void) const = 0;

        static constexpr EventFlags static_type(void) { return 0; }

        virtual std::string get_details(void) const;
        std::string get_string(void) const;

        void log_event(void) const;

        bool handled(void) const;
        void set_handled(void);

    private:
        bool m_handled;
    };

    template <typename E>
    concept AbstractEventConcept = std::derived_from<E, Event>;

    template <typename E>
    concept ConcreteEventConcept = AbstractEventConcept<E> && !std::same_as<E, Event>;

    template <AbstractEventConcept E>
    using AbstractEventHandlerFn = std::function<void(E&)>;

    template <ConcreteEventConcept E>
    using ConcreteEventHandlerFn = std::function<bool(const E&)>; // Returns true if event was handled

    template <typename E1, typename E2>
    concept is_base_event_of = AbstractEventConcept<E1> && ConcreteEventConcept<E2>
        && std::derived_from<E2, E1>;

    template <AbstractEventConcept E1>
    class EventDispatcher
    {
    public:
        EventDispatcher(E1& event)
          : event(event)
        { }

        // Calls event handler function for events of type E
        template <ConcreteEventConcept E2> requires is_base_event_of<E1, E2>
        void dispatch(const ConcreteEventHandlerFn<E2>& handler)
        {
            if (event.get_type() == E2::static_type() && !event.handled())
            {
                bool handled = handler(static_cast<E2&>(event));
                if (handled)
                    event.set_handled();
            }
        }

    private:
        E1& event;
    };
}

#endif // DRK_EVENT_HPP
