#ifndef DRK_COMPONENETS_HPP
#define DRK_COMPONENETS_HPP

#include "Core/Base.hpp"
#include "Graphics/Transform.hpp"
#include "Game/GameEvents.hpp"

namespace DrkCraft
{
    struct TransformComponent
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

#endif // DRK_COMPONENTS_HPP
