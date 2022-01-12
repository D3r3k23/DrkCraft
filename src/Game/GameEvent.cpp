#include "GameEventTypes.hpp"
#include "GameEvent.hpp"
#include "GameEvents.hpp"

#include <algorithm>

namespace DrkCraft
{
    std::deque<GameEventSubscriber*> GameEvent::s_subscribers;

    void GameEvent::post(GameEvent& event)
    {
        DRK_LOG_GAME_INFO("Posting GameEvent");
        for (auto subscriber : s_subscribers)
            if (subscriber->subscribed(event))
                subscriber->on_game_event(event);
    }

    void GameEvent::subscribe(GameEventSubscriber* subscriber)
    {
        DRK_LOG_GAME_INFO("Adding GameEvent subscriber");
        s_subscribers.push_back(subscriber);
    }

    void GameEvent::unsubscribe(GameEventSubscriber* subscriber)
    {
        DRK_LOG_GAME_INFO("Removing GameEvent subscriber");
        if (auto it = std::ranges::find(s_subscribers, subscriber); it != s_subscribers.end())
            s_subscribers.erase(it);
    }

    GameEventSubscriber::GameEventSubscriber(GameEventFlags subscriptions)
      : m_gameEventSubscriptions(subscriptions)
    {
        GameEvent::subscribe(this);
    }

    GameEventSubscriber::~GameEventSubscriber(void)
    {
        GameEvent::unsubscribe(this);
    }

    GameEventFlags GameEventSubscriber::game_event_subscriptions(void) const
    {
        return m_gameEventSubscriptions;
    }

    bool GameEventSubscriber::subscribed(const GameEvent& event) const
    {
        if (event.get_type() == game_event_subscriptions())
            return true;
        return false;
    }

    ////////// GameEventTypes.hpp //////////

    GameEventFlags to_game_event_flags(auto item)
    {
        return static_cast<GameEventFlags>(item);
    }

    bool game_event_flag_contains(GameEventFlags flags, auto item)
    {
        return to_game_event_flags(item) & flags != 0;
    }

    bool game_event_flag_equals(GameEventFlags flags, auto item)
    {
        GameEventFlags itemFlags = to_game_event_flags(item);
        return itemFlags & flags == itemFlags;
    }

    bool game_event_type_is(const GameEvent& event, auto item)
    {
        return item == event.get_type();
    }

    bool operator==(GameEventType type, GameEventFlags flags)
    {
        return game_event_flag_contains(flags, type);
    }

    bool operator==(GameEventCategory cat, GameEventFlags flags)
    {
        return game_event_flag_contains(flags, cat);
    }

    bool operator!=(GameEventType type, GameEventFlags flags)
    {
        return !game_event_flag_contains(flags, type);
    }

    bool operator!=(GameEventCategory cat, GameEventFlags flags)
    {
        return !game_event_flag_contains(flags, cat);
    }

    bool operator==(GameEventFlags flags, GameEventType type)
    {
        return game_event_flag_equals(flags, type);
    }

    bool operator==(GameEventFlags flags, GameEventCategory cat)
    {
        return game_event_flag_equals(flags, cat);
    }

    bool operator!=(GameEventFlags flags, GameEventType type)
    {
        return !game_event_flag_equals(flags, type);
    }

    bool operator!=(GameEventFlags flags, GameEventCategory cat)
    {
        return !game_event_flag_equals(flags, cat);
    }

    bool operator==(const GameEvent& event, GameEventType type)
    {
        return game_event_type_is(event, type);
    }

    bool operator==(const GameEvent& event, GameEventCategory cat)
    {
        return game_event_type_is(event, cat);
    }

    GameEventFlags operator|(GameEventFlags flags, GameEventType type)
    {
        return flags | to_game_event_flags(type);
    }

    GameEventFlags operator|(GameEventFlags flags, GameEventCategory cat)
    {
        return flags | to_game_event_flags(cat);
    }

    GameEventFlags operator|(GameEventType type, GameEventFlags flags)
    {
        return flags | type;
    }

    GameEventFlags operator|(GameEventCategory cat, GameEventFlags flags)
    {
        return flags | cat;
    }

    GameEventFlags operator|(GameEventType type1, GameEventType type2)
    {
        return to_game_event_flags(type1) | type2;
    }

    GameEventFlags operator|(GameEventCategory cat1, GameEventCategory cat2)
    {
        return to_game_event_flags(cat1) | cat2;
    }

    GameEventFlags operator|=(GameEventFlags& flags, GameEventType type)
    {
        flags = flags | type;
        return flags;
    }

    GameEventFlags operator|=(GameEventFlags flags, GameEventCategory cat)
    {
        flags = flags | cat;
        return flags;
    }

    GameEventFlags operator&(GameEventFlags flags, GameEventType type)
    {
        return flags & to_game_event_flags(type);
    }

    GameEventFlags operator&(GameEventFlags flags, GameEventCategory cat)
    {
        return flags & to_game_event_flags(cat);
    }

    GameEventFlags operator&(GameEventType type, GameEventFlags flags)
    {
        return flags & type;
    }

    GameEventFlags operator&(GameEventCategory cat, GameEventFlags flags)
    {
        return flags & cat;
    }

    GameEventFlags operator&(GameEventType type1, GameEventType type2)
    {
        return to_game_event_flags(type1) & type2;
    }

    GameEventFlags operator&(GameEventCategory cat1, GameEventCategory cat2)
    {
        return to_game_event_flags(cat1) & cat2;
    }

    GameEventFlags operator&=(GameEventFlags& flags, GameEventType type)
    {
        flags = flags & type;
        return flags;
    }

    GameEventFlags operator&=(GameEventFlags& flags, GameEventCategory cat)
    {
        flags = flags & cat;
        return flags;
    }
}
