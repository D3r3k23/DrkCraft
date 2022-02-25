#ifndef DRK_GAME_GAME_HPP
#define DRK_GAME_GAME_HPP

#include "Core/Base.hpp"
#include "Application/Events.hpp"
#include "Core/Timestep.hpp"
#include "System/AssetLibrary.hpp"
#include "Game/World/World.hpp"
#include "Game/Entity/EntityScene.hpp"
#include "Game/Systems/EntityRenderer.hpp"
#include "Game/Systems/Lighting.hpp"
#include "Game/Systems/Physics.hpp"
#include "Game/Systems/WorldRenderer.hpp"
#include "Game/World/WorldLoader.hpp"
#include "Game/Player.hpp"
#include "Game/Sky.hpp"
#include "Game/Layers/Hud.hpp"

#include "Audio/AudioSource.hpp"

#include "lib/fs.hpp"

namespace DrkCraft
{
    class Game
    {
    public:
        static const AssetList& get_asset_list(void);

        Game(AssetLibrary& assets);
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
        void update_game_time(Timestep timestep);

    private:
        AssetLibrary& m_assets;

        World m_world;
        EntityScene m_entityScene;

        WorldRendererSystem m_worldRenderer;
        EntityRendererSystem m_entityRenderer;
        LightingSystem m_lightingSystem;
        PhysicsSystem m_physicsSystem;

        PlayerController m_playerController;
        Sky m_sky;

        Ref<Hud> m_hud;

        bool m_running;
        bool m_paused;

        float m_gameTimeSeconds;
        uint m_gameTime;

        Ref<AudioSource> song;
    };
}

#endif // DRK_GAME_GAME_HPP
