#ifndef DRK_GAME_GAME_HPP
#define DRK_GAME_GAME_HPP

#include "Core/Base.hpp"
#include "Application/Events.hpp"
#include "Application/Timestep.hpp"
#include "System/AssetManager.hpp"
#include "Game/World/World.hpp"
#include "Game/Entity/EntityScene.hpp"
#include "Game/Systems/EntityRenderer.hpp"
#include "Game/Systems/Lighting.hpp"
#include "Game/Systems/Physics.hpp"
#include "Game/Systems/WorldRenderer.hpp"
#include "Game/Player.hpp"
#include "Game/Skybox.hpp"
#include "Game/Layers/Hud.hpp"

// Temp
#include "Util/Random.hpp"
#include "Graphics/detail/Util.hpp"
#include "Graphics/Renderer/Renderer.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/detail/Buffer.hpp"
#include "Audio/AudioSource.hpp"

#include "lib/fs.hpp"

namespace DrkCraft
{
    class Game
    {
    public:
        static const AssetList& get_asset_list(void);

        Game(World world, AssetManager& assets);
        ~Game(void);

        void render(void);
        void update(Timestep timestep);
        void on_event(InputEvent& event);

        bool on_key_pressed(const KeyPressedEvent& event);

        void pause(void);
        void unpause(void);
        bool is_paused(void) const;

        const PlayerController& get_player(void) const;

        void save(void);

    private:
        AssetManager& m_assets;

        World m_world;
        EntityScene m_entityScene;

        WorldRendererSystem m_worldRenderer;
        EntityRendererSystem m_entityRenderer;
        LightingSystem m_lightingSystem;
        PhysicsSystem m_physicsSystem;

        PlayerController m_playerController;
        Skybox m_skybox;

        Ref<Hud> m_hud;

        bool m_running;
        bool m_paused;

        // Temp
        Ptr<VertexBuffer> vertexBuffer;
        ShaderProgram flatColorShaderProgram;
        vec3 color;
        RandomFloatDist randomDist;

        Ref<AudioSource> song;
    };
}

#endif // DRK_GAME_GAME_HPP
