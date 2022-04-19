#include "Texture.hpp"

#include "Core/Debug/Profiler.hpp"

#include <glad/glad.h>

#include <algorithm>

namespace DrkCraft
{
    /////////////////////////
    //       Texture       //
    /////////////////////////

    Ref<Texture> Texture::from_image(const Image& image)
    {
        return {};
    }

    Ref<Texture> Texture::from_data()
    {
        return {};
    }

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

    //////////////////////////////
    //       TextureAtlas       //
    //////////////////////////////

    TextureAtlas::TextureAtlas(Ref<Texture> atlasTexture, uint numTextures)
      : m_texture(atlasTexture),
        m_numTextures(numTextures)
    { }

    Ref<Texture> TextureAtlas::get_texture(void) const
    {
        return {};
    }

    uvec2 TextureAtlas::get_coordinates(uint index, const ivec2& subtextureCoordinates)
    {
        return {};
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
        return m_numReserved++;
    }

    void TextureManager::refresh(void)
    {
        if (m_numReserved == 0)
            m_textures.clear();
        else
            m_textures.erase(m_textures.begin() + m_numReserved, m_textures.end());
    }

    void TextureManager::attach(Ref<Texture> texture, uint slot)
    {
        texture->attach(slot);
        m_textures[slot] = std::move(texture);
    }

    void TextureManager::attach(Ref<Texture> texture)
    {
        if (full())
        {
            DRK_ASSERT_DEBUG(false, "Texture slots are full");
            DRK_LOG_CORE_ERROR("Texture slots are full");
        }
        else
        {
            texture->attach(m_textures.size());
            m_textures.push_back(std::move(texture));
        }
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
