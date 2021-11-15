#include "Events.hpp"
#include "EventInfo.hpp"

#include <format>

namespace DrkCraft
{
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

    std::string Event::get_details(void) const
    {
        return "";
    }

    std::string Event::get_string(void) const
    {
        if (std::string details = get_details(); details.size() > 0)
            return std::format("{}: {}", get_name(), details);
        else
            return get_name();
    }

    ////////// Events.hpp //////////

    std::string WindowResizedEvent::get_details(void) const
    {
        return std::format("width={} height={}", width, height);
    }

    std::string FramebufferResizedEvent::get_details(void) const
    {
        return std::format("width={} height={}", width, height);
    }

    std::string WindowMovedEvent::get_details(void) const
    {
        return std::format("xPos={} yPos={}", xPos, yPos);
    }

    std::string WindowScaledEvent::get_details(void) const
    {
        return std::format("xScale={} yScale={}", xScale, yScale);
    }

    std::string KeyEvent::get_details(void) const
    {
        return std::format("KeyCode={} InputMod={}", from_key_code(key), mods);
    }

    std::string CharTypedEvent::get_details(void) const
    {
        return std::format("char={}", ch);
    }

    std::string MousePosEvent::get_details(void) const
    {
        return std::format("xPos={} yPos={}",xPos, yPos);
    }

    std::string MouseButtonEvent::get_details(void) const
    {
        return std::format("MouseCode={} xPos={} yPos={} InputMod={}", from_mouse_code(button), xPos, yPos, mods);
    }

    std::string ScrollWheelMovedEvent::get_details(void) const
    {
        return std::format("xOffset={} yOffset={}", xOffset, yOffset);
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

    bool operator==(const Event& event, EventCategory cat)
    {
        return event.get_category() == cat;
    }

    bool operator==(EventCategory cat, const Event& event)
    {
        return event == cat;
    }

    bool operator!=(const Event& event, EventCategory cat)
    {
        return !(event == cat);
    }

    bool operator!=(EventCategory cat, const Event& event)
    {
        return !(event == cat);
    }
}
