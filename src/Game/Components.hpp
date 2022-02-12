#ifndef DRK_GAME_COMPONENETS_HPP
#define DRK_GAME_COMPONENETS_HPP

#include "Core/Base.hpp"
#include "Graphics/Transform.hpp"
#include "Game/GameEvents.hpp"

namespace DrkCraft
{
    struct MeshComponent
    {

    };

    struct TransformComponent
    {

    };

    struct HitboxComponent
    {

    };

    struct LightComponent
    {

    };

    struct GameEventSubscriberComponent : public GameEventSubscriber
    {
    public:
        GameEventSubscriberComponent(const GameEventHandlerFn<GameEvent>& eventHandler);

        virtual void on_game_event(const GameEvent& event) override;

    private:
        GameEventHandlerFn<GameEvent> m_eventHandler;
    };
}

#endif // DRK_GAME_COMPONENTS_HPP
