#include "Events.hpp"

#include <format>

namespace DrkCraft
{
    void log_event(const Event& event)
    {
        switch (event.get_type())
        {
            case EventType::MouseMoved : break;
            case EventType::CharTyped  : break;
            case EventType::KeyHeld    : break;
            default: DRK_LOG_INFO("[Event] {}", std::string(event));
        }
    }

    Event::operator std::string(void) const
    {
        return get_name();
    }

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

    MouseEvent::operator std::string(void) const
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
}
