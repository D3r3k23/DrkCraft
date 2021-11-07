#ifndef GAME_HPP
#define GAME_HPP

#include "Core/Base.hpp"
#include "Application/Layer.hpp"
#include "Game/World.hpp"

namespace DrkCraft
{
    class Game : public Layer
    {
    public:

    private:
        World world;
    };
}

#endif // GAME_HPP
