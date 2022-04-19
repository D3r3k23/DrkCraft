#ifndef DRK_GAME_ENTITY_COMPONENETS_HPP
#define DRK_GAME_ENTITY_COMPONENETS_HPP

#include "Core/Base.hpp"
#include "Graphics/Transform.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Texture.hpp"
#include "Game/GameEvents.hpp"

namespace DrkCraft
{
    struct Component
    {

    };

    struct TransformComponent final : Component
    {
        Transform transform;
    };

    struct MeshComponent final : Component
    {
        Ref<Mesh> mesh;
    };

    struct TextureComponent final : Component
    {
        Ref<Texture> texture;
    };

    struct HitboxComponent final : Component
    {
        int x;
    };

    struct LightComponent final : Component
    {
        int x;
    };

    struct PlayerComponent final : Component
    {
        int x;
    };

    struct HealthComponent final : Component
    {
        int x;
    };

    struct CombatComponent final : Component
    {
        int x;
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
