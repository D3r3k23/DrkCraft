#include "Texture.hpp"

#include "Core/Debug/Profiler.hpp"

#include <glad/glad.h>

#include <algorithm>

namespace DrkCraft
{
    /////////////////////////
    //       Texture       //
    /////////////////////////

    Texture::~Texture(void)
    {
        DRK_PROFILE_FUNCTION();
        glDeleteTextures(1, &m_id);
    }

    ///////////////////////////
    //       Texture2D       //
    ///////////////////////////

    Texture2D::Texture2D(void)
    {
        DRK_PROFILE_FUNCTION();
        glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    }

    void Texture2D::bind(void) const
    {
        glBindTexture(GL_TEXTURE_2D);
    }

    void Texture2D::unbind(void) const
    {
        glBindTexture(0);
    }

    ////////////////////////////////
    //       TextureManager       //
    ////////////////////////////////

    TextureManager::TextureManager(void)
      : m_MAX_TEXTURES(get_max_textures())
    { }

    uint TextureManager::get_max_textures(void)
    {
        int textureUnits;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
        return std::min(static_cast<uint>(textureUnits), 32u);
    }
}
