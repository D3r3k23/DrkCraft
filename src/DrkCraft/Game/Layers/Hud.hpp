#ifndef DRK_GAME_LAYERS_HUD_HPP
#define DRK_GAME_LAYERS_HUD_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Application/Events.hpp"
#include "Util/Timestep.hpp"
#include "Game/Player.hpp"

#include <functional>
#include <concepts>

namespace DrkCraft::Game
{
    class Hud : public Layer
    {
    public:
        Hud(const PlayerController& player, bool activate);
        virtual ~Hud(void);

        virtual void on_attach(void) override;
        virtual void on_detach(void) override;

        virtual void on_update(Timestep timestep) override;
        virtual void on_render(void) override;
        virtual void on_event(Event& event) override;

    private:
        const PlayerController& m_player;
    };
}

#endif // DRK_GAME_LAYERS_HUD_HPP
