#ifndef DRK_GAME_GAME_EVENT_INFO
#define DRK_GAME_GAME_EVENT_INFO

// Do not include directly, use Game/GameEvents.hpp instead

#include "Core/Base.hpp"

namespace DrkCraft
{
    struct GameEvent;

    using GameEventFlags = uint32;

    enum class GameEventType : GameEventFlags
    {
        None = 0,

        // Player Events
        PlayerSpawn = 1 << 0,
        PlayerDeath = 1 << 1,

        // World Events
        WorldUpdate = 1 << 2
    };

    enum class GameEventCategory : GameEventFlags
    {
        None = 0,

        Player = (GameEventFlags)GameEventType::PlayerSpawn
               | (GameEventFlags)GameEventType::PlayerDeath,

        World = (GameEventFlags)GameEventType::WorldUpdate,

        GameEvent = (GameEventFlags)Player
                  | (GameEventFlags)World
    };

    GameEventFlags to_game_event_flags(auto item);
    bool game_event_has_flags(const GameEvent& event, GameEventFlags flags);

    bool operator==(const GameEvent& event, GameEventFlags flags);
    bool operator==(const GameEvent& event, GameEventType type);
    bool operator==(const GameEvent& event, GameEventCategory cat);

    bool operator!=(const GameEvent& event, GameEventFlags flags);
    bool operator!=(const GameEvent& event, GameEventType type);
    bool operator!=(const GameEvent& event, GameEventCategory cat);

    GameEventFlags operator|(GameEventFlags flags, GameEventType type);
    GameEventFlags operator|(GameEventFlags flags, GameEventCategory cat);

    GameEventFlags operator|(GameEventType type,    GameEventFlags flags);
    GameEventFlags operator|(GameEventCategory cat, GameEventFlags flags);

    GameEventFlags operator|(GameEventType type,    GameEventCategory cat);
    GameEventFlags operator|(GameEventCategory cat, GameEventType type);

    GameEventFlags operator|(GameEventType type1,    GameEventType type2);
    GameEventFlags operator|(GameEventCategory cat1, GameEventCategory cat2);

    GameEventFlags operator|=(GameEventFlags& flags, GameEventType type);
    GameEventFlags operator|=(GameEventFlags& flags, GameEventCategory cat);
}

#endif // DRK_GAME_GAME_EVENT_INFO
