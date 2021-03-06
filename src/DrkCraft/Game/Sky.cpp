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
        m_hour(0)
    {

    }

    void Sky::render(void)
    {
        m_skybox.render();
    }

    void Sky::update(const GameTime& gameTime)
    {
        uint hour = gameTime.time();
        if (hour != m_hour)
        {
            m_hour = hour;
            // Update position of sun/moon
        }
    }
}
