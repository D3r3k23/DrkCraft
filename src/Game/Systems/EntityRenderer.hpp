#ifndef DRK_GAME_SYSTEMS_ENTITY_RENDERER_HPP
#define DRK_GAME_SYSTEMS_ENTITY_RENDERER_HPP

#include "Core/Base.hpp"
#include "Game/System.hpp"
#include "Application/Timestep.hpp"
#include "Game/Entity/EntityScene.hpp"

namespace DrkCraft
{
    class EntityRendererSystem : public System
    {
    public:
        EntityRendererSystem(EntityScene& scene);
        ~EntityRendererSystem(void) = default;

        virtual void render(void) override;
    };
}

#endif // DRK_GAME_SYSTEMS_ENTITY_RENDERER_HPP
