#ifndef DRK_APPLICATION_EVENT_GENERATOR_HPP
#define DRK_APPLICATION_EVENT_GENERATOR_HPP

#include "Core/Base.hpp"
#include "Application/Events.hpp"
#include "System/Window.hpp"

struct GLFWwindow;

namespace DrkCraft
{
    class EventGenerator
    {
    public:
        EventGenerator(Window& window);
        ~EventGenerator(void);

        void register_event_handler(const AbstractEventHandlerFn<Event>& handler);
        void poll_events(void);

    private:
        void set_window_callbacks(void);

    private:
        Window& m_window;
        AbstractEventHandlerFn<Event> m_handler;
    };
}

#endif // DRK_APPLICATION_EVENT_GENERATOR_HPP
