#include "Components.hpp"

namespace DrkCraft
{
    GameEventSubscriberComponent::GameEventSubscriberComponent(GameEventFlags subscriptions,
        const GameEventHandlerFn<GameEvent>& eventHandler)
      : GameEventSubscriber(subscriptions),
        m_eventHandler(eventHandler)
    { }

    void GameEventSubscriberComponent::on_game_event(GameEvent& event)
    {
        m_eventHandler(event);
    }

    struct ExampleEntity
    {
        GameEventSubscriberComponent gameEventSubscriber;
        ExampleEntity(void)
          : gameEventSubscriber(to_game_event_flags(GameEventCategory::Player), DRK_BIND_FN(on_event))
        { }
        void on_event(const GameEvent& event)
        {
            GameEventDispatcher ed(event);
            ed.dispatch<PlayerSpawnEvent>(DRK_BIND_FN(on_player_spawn));
        }
        void on_player_spawn(const PlayerSpawnEvent& event)
        {
            // playerLocation = event.location;
            // walk_to(playerLocation);
        }
    };
}
