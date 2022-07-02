#ifndef DRK_GAME_GAME_HPP
#define DRK_GAME_GAME_HPP

// Layer
#include "Core/Base.hpp"
#include "Application/Events.hpp"
#include "Util/Timestep.hpp"
#include "System/Library.hpp"

// Data
#include "Game/World/World.hpp"
#include "Game/Entity/Scene.hpp"
#include "Game/GameEvent.hpp"

// Systems
#include "Game/GameSystem.hpp"
#include "Game/Systems/EntityRenderer.hpp"
#include "Game/Systems/Lighting.hpp"
#include "Game/Systems/Physics.hpp"
#include "Game/Systems/WorldRenderer.hpp"

// Game
#include "Game/World/WorldLoader.hpp"
#include "Game/Player.hpp"
#include "Game/Sky.hpp"
#include "Game/Layers/Hud.hpp"
#include "Game/Util/GameTime.hpp"

#include "Audio/AudioSource.hpp"

#include "Lib/fs.hpp"

namespace DrkCraft::Game
{
    class Game
    {
    public:
        static const AssetList& get_asset_list(void);

        Game(Library& assets);
        ~Game(void);

        void render(void);
        void update(Timestep timestep);
        void on_event(InputEvent& event);

        bool on_key_pressed(const KeyPressedEvent& event);

        void pause(void);
        void unpause(void);
        bool is_paused(void) const;

        const PlayerController& get_player(void) const;
        const GameTime& get_time(void) const;

        void save(void);

    private:

    private:
        Library& m_assets;

        World m_world;
        Scene m_scene;

        GameEventQueue m_gameEventQueue;

        GameSystemData m_systemData;

        WorldRendererSystem m_worldRenderer;
        EntityRendererSystem m_entityRenderer;
        LightingSystem m_lightingSystem;
        PhysicsSystem m_physicsSystem;

        PlayerController m_playerController;
        Sky m_sky;

        Ref<Hud> m_hud;

        bool m_running;
        bool m_paused;

        GameTime m_gameTime;

        Ref<AudioSource> song;
    };
}

#endif // DRK_GAME_GAME_HPP
