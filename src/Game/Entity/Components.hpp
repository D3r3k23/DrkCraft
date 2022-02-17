#ifndef DRK_GAME_ENTITY_COMPONENETS_HPP
#define DRK_GAME_ENTITY_COMPONENETS_HPP

#include "Core/Base.hpp"
#include "Game/Entity/Component.hpp"
#include "Graphics/Transform.hpp"
#include "Game/GameEvents.hpp"

namespace DrkCraft
{
    struct TransformComponent : Component
    {

    };

    struct MeshComponent : Component
    {

    };

    struct TextureComponent : Component
    {

    };

    struct HitboxComponent : Component
    {

    };

    struct LightComponent : Component
    {

    };

    // Uses pointers
    // Could use entt systems instead
    struct GameEventSubscriberComponent : public GameEventSubscriber
    {
    public:
        GameEventSubscriberComponent(const GameEventHandlerFn<GameEvent>& eventHandler);

        virtual void on_game_event(const GameEvent& event) override;

    private:
        GameEventHandlerFn<GameEvent> m_eventHandler;
    };
}

#endif // DRK_GAME_ENTITY_COMPONENTS_HPP
