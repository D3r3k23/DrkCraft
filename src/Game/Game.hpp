#ifndef DRK_GAME_GAME_HPP
#define DRK_GAME_GAME_HPP

#include "Core/Base.hpp"
#include "Application/Events.hpp"
#include "Application/Timestep.hpp"
#include "System/AssetManager.hpp"
#include "Game/Layers/DebugOverlay.hpp"
#include "Game/World.hpp"
#include "Game/Player.hpp"

// Temp
#include "Util/Random.hpp"
#include "Graphics/detail/Util.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/detail/Buffer.hpp"
#include "Audio/AudioSource.hpp"

#include "lib/fs.hpp"

namespace DrkCraft
{
    class Game
    {
    public:
        Game(Ptr<World> world, AssetManager& assets, Ref<DebugOverlay> debugLayer);
        ~Game(void);

        void update(Timestep timestep);
        void render(void);
        void on_event(Event& event);

        bool on_key_pressed(const KeyPressedEvent& event);

        void pause(void);
        void unpause(void);
        bool is_paused(void) const;

        void save(void);

    private:
        AssetManager& m_assets;

        Ptr<World> m_world;

        Player m_player;

        bool m_running;
        bool m_paused;

        Ref<DebugOverlay> m_debugOverlay;

        // Temp
        Ptr<VertexBuffer> vertexBuffer;
        ShaderProgram flatColorShaderProgram;
        vec3 color;
        RandomFloatDist randomDist;

        Ref<AudioSource> song;
    };
}

#endif // DRK_GAME_GAME_HPP
