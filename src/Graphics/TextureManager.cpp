#include "TextureManager.hpp"

#include <glad/glad.h>

namespace DrkCraft
{
    TextureManager::TextureManager(void)
      : m_MAX_TEXTURES(get_max_textures())
    { }

    uint TextureManager::get_max_textures(void)
    {
        int textureUnits;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
        return static_cast<uint>(textureUnits);
    }
}
