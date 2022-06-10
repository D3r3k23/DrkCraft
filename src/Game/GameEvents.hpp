#ifndef DRK_GAME_GAME_EVENTS_HPP
#define DRK_GAME_GAME_EVENTS_HPP

#include "Core/Base.hpp"
#include "Game/GameEvent.hpp"

#define DRK_GAME_EVENT_TYPE_INFO(name)                                          \
    virtual const char*   get_name(void) const override { return #name"Event"; } \
    virtual GameEventType get_type(void) const override { return static_type(); } \
    static constexpr GameEventType static_type(void)    { return GameEventType::name; }

#define DRK_GAME_EVENT_CATEGORY_INFO(name) \
    static constexpr GameEventCategory static_type(void) { return GameEventCategory::name; }

namespace DrkCraft::Game
{
    struct PlayerEvent : GameEvent
    {
        DRK_GAME_EVENT_CATEGORY_INFO(Player)
    };

    struct PlayerSpawnEvent final : PlayerEvent
    {
        DRK_GAME_EVENT_TYPE_INFO(PlayerSpawn)
    };

    struct PlayerDeathEvent final : PlayerEvent
    {
        DRK_GAME_EVENT_TYPE_INFO(PlayerDeath)
    };

    struct WorldEvent final : GameEvent
    {
        DRK_GAME_EVENT_CATEGORY_INFO(World)
    };
}

#undef DRK_GAME_EVENT_TYPE_INFO

#endif // DRK_GAME_GAME_EVENTS_HPP
