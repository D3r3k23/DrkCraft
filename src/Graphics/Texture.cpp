#include "Texture.hpp"

#include "Core/Debug/Profiler.hpp"

#include <glad/glad.h>

#include <algorithm>

namespace DrkCraft
{
    /////////////////////////
    //       Texture       //
    /////////////////////////

    /////////////////////////
    //       Texture       //
    /////////////////////////

    Texture::Texture(void)
    {
        DRK_PROFILE_FUNCTION();
        glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    }

    Texture::~Texture(void)
    {
        DRK_PROFILE_FUNCTION();
        glDeleteTextures(1, &m_id);
    }

    void Texture::attach(uint slot) const
    {
        DRK_ASSERT_DEBUG(!attached(), "Texture is already attached");
        m_slot = slot;
        glBindTextureUnit(m_id, slot);
    }

    void Texture::detach(void) const
    {
        if (m_slot)
            glBindTextureUnit(0, *m_slot);
    }

    bool Texture::attached(void) const
    {
        return m_slot.has_value();
    }

    ////////////////////////////////
    //       TextureManager       //
    ////////////////////////////////

    TextureManager::TextureManager(void)
      : m_maxTextures(get_max_textures())
    {
        m_textures.reserve(m_maxTextures);
    }

    uint TextureManager::reserve(void)
    {
        DRK_ASSERT_DEBUG(empty(), "Texture slots are not empty");
        return m_nReserved++;
    }

    void TextureManager::refresh(void)
    {
        if (m_nReserved == 0)
            m_textures.clear();
        else
            m_textures.erase(m_textures.begin() + m_nReserved, m_textures.end());
    }

    void TextureManager::emplace(uint slot, Ref<Texture> texture)
    {

    }

    void TextureManager::push(Ref<Texture> texture)
    {
        DRK_ASSERT_DEBUG(!full(), "Texture slots are full");
    }

    uint TextureManager::count(void) const
    {
        return m_textures.size();
    }

    bool TextureManager::full(void) const
    {
        return count() >= m_maxTextures;
    }

    bool TextureManager::empty(void) const
    {
        return m_textures.empty();
    }

    uint TextureManager::get_max_textures(void)
    {
        int textureUnits;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
        return std::min(static_cast<uint>(textureUnits), 32u);
    }
}
