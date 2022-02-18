#ifndef DRK_GAME_ENTITY_COMPONENETS_HPP
#define DRK_GAME_ENTITY_COMPONENETS_HPP

#include "Core/Base.hpp"
#include "Graphics/Transform.hpp"
#include "Game/GameEvents.hpp"

namespace DrkCraft
{
    struct Component
    {

    };

    struct TransformComponent final : Component
    {

    };

    struct MeshComponent final : Component
    {

    };

    struct TextureComponent final : Component
    {

    };

    struct HitboxComponent final : Component
    {

    };

    struct LightComponent final : Component
    {

    };

    struct PlayerComponent final : Component
    {

    };

    struct InventoryComponent final : Component
    {

    };

    struct HealthComponent final : Component
    {

    };

    struct CombatComponent final : Component
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
