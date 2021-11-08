#include "Events.hpp"

#include <format>

namespace DrkCraft
{
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
        return std::format("{}: KeyCode={}", get_name(), get_code(key));
    }

    CharTypedEvent::operator std::string(void) const
    {
        return std::format("{}: char={}", get_name(), ch);
    }

    MouseButtonEvent::operator std::string(void) const
    {
        return std::format("{}: MouseCode={}", get_name(), get_code(button));
    }

    MouseMovedEvent::operator std::string(void) const
    {
        return std::format("{}: xPos={} yPos{}", get_name(), xPos, yPos);
    }

    MouseScrolledEvent::operator std::string(void) const
    {
        return std::format("{}: xOffset={} yOffset{}", get_name(), xOffset, yOffset);
    }
}
