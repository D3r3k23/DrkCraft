#ifndef DRK_GAME_EVENTS_HPP
#define DRK_GAME_EVENTS_HPP

#include "Core/Base.hpp"
#include "GameEvent.hpp"

#define DRK_GAME_EVENT_TYPE_INFO(flagType, name)           \
    virtual const char* get_name(void) const override       \
        { return #name"Event"; }                             \
    virtual GameEventFlags get_type(void) const override      \
        { return static_cast<GameEventFlags>(static_type()); } \
    static constexpr flagType static_type(void)                 \
        { return flagType::name; }

namespace DrkCraft
{
    struct PlayerEvent : public GameEvent
    {
        DRK_GAME_EVENT_TYPE_INFO(GameEventCategory, Player)
    };

    struct PlayerSpawnEvent : public PlayerEvent
    {
        DRK_GAME_EVENT_TYPE_INFO(GameEventType, PlayerSpawn)
    };

    struct PlayerDeathEvent : public PlayerEvent
    {
        DRK_GAME_EVENT_TYPE_INFO(GameEventType, PlayerDeath)
    };
}

#undef DRK_GAME_EVENT_TYPE_INFO

#endif // DRK_GAME_EVENTS_HPP
