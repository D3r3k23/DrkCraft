#include "GameEventTypes.hpp"
#include "GameEvent.hpp"
#include "GameEvents.hpp"

#include <algorithm>

namespace DrkCraft::Game
{
    ///////////////////////////////////
    //         GameEvent.hpp         //
    ///////////////////////////////////

    std::deque<GameEventSubscriber*> GameEvent::s_subscribers;

    void GameEvent::post(const GameEvent& event)
    {
        DRK_LOG_GAME_INFO("Posting GameEvent: {}", event.get_name());
        for (auto subscriber : s_subscribers)
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

    GameEventSubscriber::GameEventSubscriber(void)
    {
        GameEvent::subscribe(this);
    }

    GameEventSubscriber::~GameEventSubscriber(void)
    {
        GameEvent::unsubscribe(this);
    }

    ////////////////////////////////////
    //         GameEvents.hpp         //
    ////////////////////////////////////



    ////////////////////////////////////////
    //         GameEventTypes.hpp         //
    ////////////////////////////////////////

    GameEventFlags to_game_event_flags(GameEventType type)
    {
        return static_cast<GameEventFlags>(type);
    }

    GameEventFlags to_game_event_flags(GameEventCategory cat)
    {
        return static_cast<GameEventFlags>(cat);
    }

    bool game_event_has_flags(const GameEvent& event, GameEventFlags flags)
    {
        GameEventFlags eventTypeFlag = to_game_event_flags(event.get_type());
        return (eventTypeFlag & flags) == eventTypeFlag;
    }

    bool operator==(const GameEvent& event, GameEventFlags flags)
    {
        return game_event_has_flags(event, flags);
    }

    bool operator==(const GameEvent& event, GameEventType type)
    {
        return event == to_game_event_flags(type);
    }

    bool operator==(const GameEvent& event, GameEventCategory cat)
    {
        return event == to_game_event_flags(cat);
    }

    bool operator!=(const GameEvent& event, GameEventFlags flags)
    {
        return !(event == flags);
    }

    bool operator!=(const GameEvent& event, GameEventType type)
    {
        return !(event == type);
    }

    bool operator!=(const GameEvent& event, GameEventCategory cat)
    {
        return !(event == cat);
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

    GameEventFlags operator|(GameEventType type, GameEventCategory cat)
    {
        return to_game_event_flags(type) | cat;
    }

    GameEventFlags operator|(GameEventCategory cat, GameEventType type)
    {
        return type | cat;
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
}
