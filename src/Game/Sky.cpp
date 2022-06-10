#include "Sky.hpp"

namespace DrkCraft::Game
{
    ////////////////////////
    //       Skybox       //
    ////////////////////////

    Skybox::Skybox(void)
    {

    }

    void Skybox::render(void)
    {

    }

    Sky::Sky(void)
      : m_skybox(),
        m_gameTime(0)
    {

    }

    void Sky::render(void)
    {
        m_skybox.render();
    }

    void Sky::update(uint gameTime)
    {
        if (gameTime != m_gameTime)
        {
            // Update position of sun/moon
        }
    }
}
