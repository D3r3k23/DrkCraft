#include "Components.hpp"

namespace DrkCraft::Game
{
    GameEventSubscriberComponent::GameEventSubscriberComponent(const GameEventHandlerFn<GameEvent>& eventHandler)
      : m_eventHandler(eventHandler)
    { }

    void GameEventSubscriberComponent::on_game_event(const GameEvent& event)
    {
        m_eventHandler(event);
    }

    struct ExampleEntity
    {
        GameEventSubscriberComponent gameEventSubscriber;

        ExampleEntity(void)
          : gameEventSubscriber(DRK_BIND_FN(on_event))
        { }

        void on_event(const GameEvent& event)
        {
            GameEventDispatcher ed(event);
            ed.dispatch<PlayerEvent>(DRK_BIND_FN(on_player_event));
        }

        void on_player_event(const PlayerEvent& event)
        {
            GameEventDispatcher ed(event);
            ed.dispatch<PlayerSpawnEvent>(DRK_BIND_FN(on_player_spawn));
            ed.dispatch<PlayerDeathEvent>(DRK_BIND_FN(on_player_death));
        }

        void on_player_spawn(const PlayerSpawnEvent& event)
        {
            // playerLocation = event.location;
            // walk_to(playerLocation);
        }

        void on_player_death(const PlayerDeathEvent& event)
        {
            // GameEvent::post(PlayerSpawnEvent());
        }
    };
}
