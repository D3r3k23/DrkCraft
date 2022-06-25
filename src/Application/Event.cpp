#include "EventTypes.hpp"
#include "Event.hpp"
#include "Events.hpp"

#include <fmt/format.h>
#include <magic_enum.hpp>

namespace DrkCraft
{
    ///////////////////////////////
    //         Event.hpp         //
    ///////////////////////////////

    Event::Event(void)
      : m_handled(false)
    { }

    string Event::get_details(void) const
    {
        return "";
    }

    string Event::get_string(void) const
    {
        if (string details = get_details(); details.size() > 0)
            return fmt::format("{}: {}", get_name(), details);
        else
            return get_name();
    }

    void Event::log_event(void) const
    {
        auto msg = fmt::format("[{0}:{1}handled] {2}", get_name(), !m_handled ? "un" : "", get_details());

        switch (static_cast<EventType>(get_type()))
        {
            case EventType::MouseMoved:
                break;
            case EventType::WindowResized:
            case EventType::FramebufferResized:
            case EventType::WindowRefreshed:
            case EventType::WindowMoved:
            case EventType::CharTyped:
            case EventType::KeyHeld:
                DRK_LOG_EVENT_TRACE(msg);
                break;
            default:
                DRK_LOG_EVENT_DEBUG(msg);
        }
    }

    bool Event::handled(void) const
    {
        return m_handled;
    }

    void Event::set_handled(void)
    {
        m_handled = true;
    }

    ////////////////////////////////
    //         Events.hpp         //
    ////////////////////////////////

    string WindowResizedEvent::get_details(void) const
    {
        return fmt::format("width={} height={}", width, height);
    }

    string FramebufferResizedEvent::get_details(void) const
    {
        return fmt::format("width={} height={}", width, height);
    }

    string WindowMovedEvent::get_details(void) const
    {
        return fmt::format("xPos={} yPos={}", xPos, yPos);
    }

    string WindowRestoredEvent::get_details(void) const
    {
        return fmt::format("{}", magic_enum::enum_name(source));
    }

    string WindowScaledEvent::get_details(void) const
    {
        return fmt::format("xScale={} yScale={}", xScale, yScale);
    }

    InputCode InputEvent::get_input_code(void) const
    {
        return {};
    }

    string KeyEvent::get_details(void) const
    {
        return fmt::format("KeyCode={} InputMod={}", from_key_code(key), mods);
    }

    InputCode KeyEvent::get_input_code(void) const
    {
        return { key };
    }

    string CharTypedEvent::get_details(void) const
    {
        return fmt::format("char='{}'", ch);
    }

    string MousePosEvent::get_details(void) const
    {
        return fmt::format("xPos={} yPos={}",xPos, yPos);
    }

    string MouseButtonEvent::get_details(void) const
    {
        return fmt::format("MouseCode={} xPos={} yPos={} InputMod={}", from_mouse_code(button), xPos, yPos, mods);
    }

    InputCode MouseButtonEvent::get_input_code(void) const
    {
        return { button };
    }

    string ScrollWheelMovedEvent::get_details(void) const
    {
        return fmt::format("xOffset={} yOffset={}", xOffset, yOffset);
    }

    string MonitorEvent::get_details(void) const
    {
        return fmt::format("number={}", number);
    }

    ////////////////////////////////////
    //         EventTypes.hpp         //
    ////////////////////////////////////

    EventFlags to_event_flags(EventType type)
    {
        return static_cast<EventFlags>(type);
    }

    EventFlags to_event_flags(EventCategory cat)
    {
        return static_cast<EventFlags>(cat);
    }

    bool event_flags_has_event(const Event& event, EventFlags flags)
    {
        EventFlags eventTypeFlag = to_event_flags(event.get_type());
        return (eventTypeFlag & flags) == eventTypeFlag;
    }

    bool operator==(const Event& event, EventFlags flags)
    {
        return event_flags_has_event(event, flags);
    }

    bool operator==(const Event& event, EventType type)
    {
        return event == to_event_flags(type);
    }

    bool operator==(const Event& event, EventCategory cat)
    {
        return event == to_event_flags(cat);
    }

    bool operator!=(const Event& event, EventFlags flags)
    {
        return !(event == flags);
    }

    bool operator!=(const Event& event, EventType type)
    {
        return !(event == type);
    }

    bool operator!=(const Event& event, EventCategory cat)
    {
        return !(event == cat);
    }

    EventFlags operator|(EventFlags flags, EventType type)
    {
        return flags | to_event_flags(type);
    }

    EventFlags operator|(EventFlags flags, EventCategory cat)
    {
        return flags | to_event_flags(cat);
    }

    EventFlags operator|(EventType type, EventFlags flags)
    {
        return flags | type;
    }

    EventFlags operator|(EventCategory cat, EventFlags flags)
    {
        return flags | cat;
    }

    EventFlags operator|(EventType type, EventCategory cat)
    {
        return to_event_flags(type) | cat;
    }

    EventFlags operator|(EventCategory cat, EventType type)
    {
        return type | cat;
    }

    EventFlags operator|(EventType type1, EventType type2)
    {
        return to_event_flags(type1) | type2;
    }

    EventFlags operator|(EventCategory cat1, EventCategory cat2)
    {
        return to_event_flags(cat1) | cat2;
    }

    EventFlags operator|=(EventFlags& flags, EventType type)
    {
        flags = flags | type;
        return flags;
    }

    EventFlags operator|=(EventFlags flags, EventCategory cat)
    {
        flags = flags | cat;
        return flags;
    }
}
