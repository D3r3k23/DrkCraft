#ifndef DRK_GAME_EVENT_INFO
#define DRK_GAME_EVENT_INFO

// Do not include directly, use Game/GameEvents.hpp instead

#include "Core/Base.hpp"

namespace DrkCraft
{
    class GameEvent;

    using GameEventFlags = uint;

    enum class GameEventType : GameEventFlags
    {
        None = 0,

        // Player Events
        PlayerSpawn = 1 << 0,
        PlayerDeath = 1 << 1

        // World Events
    };

    enum class GameEventCategory : GameEventFlags
    {
        None = 0,

        Player = (GameEventFlags)GameEventType::PlayerSpawn
               | (GameEventFlags)GameEventType::PlayerDeath
    };

    GameEventFlags to_game_event_flags(auto item);

    bool game_event_flag_contains(GameEventFlags flags, auto item);
    bool game_event_flag_equals(GameEventFlags flags, auto item);

    bool game_event_type_is(const GameEvent& event, auto item);

    bool operator==(GameEventType type, GameEventFlags flags);
    bool operator==(GameEventCategory cat, GameEventFlags flags);

    bool operator!=(GameEventType type, GameEventFlags flags);
    bool operator!=(GameEventCategory cat, GameEventFlags flags);

    bool operator==(GameEventFlags flags, GameEventType type);
    bool operator==(GameEventFlags flags, GameEventCategory cat);

    bool operator!=(GameEventFlags flags, GameEventType type);
    bool operator!=(GameEventFlags flags, GameEventCategory cat);

    bool operator==(const GameEvent& event, GameEventType type);
    bool operator==(const GameEvent& event, GameEventCategory cat);

    GameEventFlags operator|(GameEventFlags flags, GameEventType type);
    GameEventFlags operator|(GameEventFlags flags, GameEventCategory cat);

    GameEventFlags operator|(GameEventType type, GameEventFlags flags);
    GameEventFlags operator|(GameEventCategory cat, GameEventFlags flags);

    GameEventFlags operator|(GameEventType type1, GameEventType type2);
    GameEventFlags operator|(GameEventCategory cat1, GameEventCategory cat2);

    GameEventFlags operator|=(GameEventFlags& flags, GameEventType type);
    GameEventFlags operator|=(GameEventFlags& flags, GameEventCategory cat);

    GameEventFlags operator&(GameEventFlags flags, GameEventType type);
    GameEventFlags operator&(GameEventFlags flags, GameEventCategory cat);

    GameEventFlags operator&(GameEventType type, GameEventFlags flags);
    GameEventFlags operator&(GameEventCategory cat, GameEventFlags flags);

    GameEventFlags operator&(GameEventType type1, GameEventType type2);
    GameEventFlags operator&(GameEventCategory cat1, GameEventCategory cat2);

    GameEventFlags operator&=(GameEventFlags& flags, GameEventType type);
    GameEventFlags operator&=(GameEventFlags& flags, GameEventCategory cat);
}

#endif // DRK_GAME_EVENT_INFO
