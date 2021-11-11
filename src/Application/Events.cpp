#include "Events.hpp"
#include "EventInfo.hpp"

#include <format>

namespace DrkCraft
{
    void log_event(const Event& event)
    {
        switch (event.get_type())
        {
            case EventType::WindowResize : break;
            case EventType::WindowMoved  : break;
            case EventType::MouseMoved   : break;
            case EventType::CharTyped    : break;
            case EventType::KeyHeld      : break;
            default: DRK_LOG_INFO("[Event] {}", std::string(event));
        }
    }

    Event::Event(void)
      : m_handled(false)
    { }

    bool Event::handled(void) const
    {
        return m_handled;
    }

    void Event::set_handled(void)
    {
        m_handled = true;
    }

    Event::operator std::string(void) const
    {
        return get_name();
    }

    ////////// Events.hpp //////////

    WindowResizeEvent::operator std::string(void) const
    {
        return std::format("{}: width={} height={}", get_name(), width, height);
    }

    WindowMovedEvent::operator std::string(void) const
    {
        return std::format("{}: xPos={} yPos{}", get_name(), xPos, yPos);
    }

    KeyEvent::operator std::string(void) const
    {
        return std::format("{}: KeyCode={} InputMod={}", get_name(), from_key_code(key), mods);
    }

    CharTypedEvent::operator std::string(void) const
    {
        return std::format("{}: char={}", get_name(), ch);
    }

    MousePosEvent::operator std::string(void) const
    {
        return std::format("{}: xPos={} yPos{}", get_name(), xPos, yPos);
    }

    MouseButtonEvent::operator std::string(void) const
    {
        return std::format("{}: MouseCode={} xPos={} yPos={} InputMod={}", get_name(), from_mouse_code(button), xPos, yPos, mods);
    }

    ScrollWheelMovedEvent::operator std::string(void) const
    {
        return std::format("{}: xOffset={} yOffset{}", get_name(), xOffset, yOffset);
    }

    ////////// EventInfo.hpp //////////

    EventCategoryFlags operator|(EventCategoryFlags flags, EventCategory cat)
    {
        return flags | static_cast<EventCategoryFlags>(cat);
    }

    EventCategoryFlags operator|(EventCategory cat, EventCategoryFlags flags)
    {
        return flags | cat;
    }

    EventCategoryFlags operator|(EventCategory cat1, EventCategory cat2)
    {
        return static_cast<EventCategoryFlags>(cat1) | cat2;
    }

    bool operator==(EventCategory cat, EventCategoryFlags flags)
    {
        return static_cast<EventCategoryFlags>(cat) & flags;
    }

    bool operator==(EventCategoryFlags flags, EventCategory cat)
    {
        return cat == flags;
    }

    bool operator!=(EventCategory cat, EventCategoryFlags flags)
    {
        return !(cat == flags);
    }

    bool operator!=(EventCategoryFlags flags, EventCategory cat)
    {
        return !(cat == flags);
    }

    bool operator==(const Event& event, EventCategoryFlags flags)
    {
        return event.get_category() == flags;
    }

    bool operator==(EventCategoryFlags flags, const Event& event)
    {
        return event == flags;
    }

    bool operator!=(const Event& event, EventCategoryFlags flags)
    {
        return !(event == flags);
    }

    bool operator!=(EventCategoryFlags flags, const Event& event)
    {
        return !(event == flags);
    }
}
