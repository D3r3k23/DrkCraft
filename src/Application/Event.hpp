#ifndef DRK_EVENT_HPP
#define DRK_EVENT_HPP

#include "Core/Base.hpp"
#include "Input.hpp"

#include <string>
#include <functional>
#include <utility>

#define DRK_BIND_EVENT_FN(fn) [this](auto&& ... args) -> decltype(auto) \
{                                                                       \
    return this->fn(std::forward<decltype(args)>(args)...);             \
}

namespace DrkCraft
{
    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        WindowMoved,
        WindowFocused,
        WindowFocusLost,
        WindowMinimized, // Maximized
        WindowRestored,
        KeyPressed,
        KeyHeld,
        KeyReleased,
        CharTyped,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    struct Event
    {
        ~Event(void) = default;

        virtual EventType   get_type(void) const = 0;
        virtual const char* get_name(void) const = 0;

        virtual operator std::string(void) const;

        bool handled = false;
    };

    using EventHandlerFn = std::function<void(Event&)>;

    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event) : event(event) { }

        template <typename E>
        bool dispatch(const auto handler) // Event handler functions return true if handled
        {
            if (event.get_type() == E::static_type())
            {
                event.handled = handler(static_cast<E&>(event));
                return true;
            }
            else
                return false;
        }

    private:
        Event& event;
    };
}

#endif // DRK_EVENT_HPP
