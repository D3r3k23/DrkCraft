#ifndef DRK_EVENT_HPP
#define DRK_EVENT_HPP

// Probably should not include directly, use Application/Events.hpp instead

#include "Core/Base.hpp"
#include "EventInfo.hpp"

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
        ~Event(void) = default;

        Event(const Event&) = delete;
        Event(Event&&) = delete;
        Event& operator=(const Event&) = delete;
        Event& operator=(Event&&) = delete;

        virtual EventType   get_type(void) const = 0;
        virtual const char* get_name(void) const = 0;
        virtual EventCategoryFlags get_category(void) const = 0;

        virtual std::string get_details(void) const;

        std::string get_string(void) const;

        bool handled(void) const;
        void set_handled(void);

    private:
        bool m_handled;
    };

    template <typename E>
    concept AbstractEventConcept = std::derived_from<E, Event>;

    template <typename E>
    concept ConcreteEventConcept = AbstractEventConcept<E> && !std::same_as<E, Event>;

    using AbstractEventHandlerFn = std::function<void(Event&)>;

    template <ConcreteEventConcept E>
    using ConcreteEventHandlerFn = std::function<bool(const E&)>;

    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event)
          : event(event)
        { }

        // Calls event handler function for events of type E
        // Handlers return true if event handled
        template <ConcreteEventConcept E>
        void dispatch(const ConcreteEventHandlerFn<E>& handler)
        {
            if (!event.handled() && event.get_type() == E::static_type())
            {
                bool handled = handler(static_cast<E&>(event));
                if (handled)
                    event.set_handled();
            }
        }

    private:
        Event& event;
    };
}

#endif // DRK_EVENT_HPP
