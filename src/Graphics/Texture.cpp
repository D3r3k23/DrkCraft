#include "Texture.hpp"

#include "Core/Debug/Profiler.hpp"

#include <glad/glad.h>

#include <algorithm>

namespace DrkCraft
{
    /////////////////////////
    //       Texture       //
    /////////////////////////

    namespace
    {
        GLenum to_gl_internal_format(TextureFormat format)
        {
            switch (format)
            {
                case TextureFormat::RGB  : return GL_RGB8;
                case TextureFormat::RGBA : return GL_RGBA8;
                default:
                    DRK_ASSERT_DEBUG(false, "Unknown TextureFormat");
                    return 0;
            }
        }

        GLenum to_gl_data_format(TextureFormat format)
        {
            switch (format)
            {
                case TextureFormat::RGB  : return GL_RGB;
                case TextureFormat::RGBA : return GL_RGBA;
                default:
                    DRK_ASSERT_DEBUG(false, "Unknown TextureFormat");
                    return 0;
            }
        }
    }

    Ref<Texture> Texture::from_image(const Image& image)
    {
        TextureFormat format = [&]
        {
            switch (image.channels())
            {
                case 3: return TextureFormat::RGB;
                case 4: return TextureFormat::RGBA;
                default:
                    DRK_ASSERT_CORE(false, "{} channels in image", image.channels());
                    return TextureFormat::None;
            }
        }();

        return Ref<Texture>(new Texture(image.data(), image.size(), format));
    }

    Ref<Texture> Texture::from_data(const uint8* data, const uvec2& size, TextureFormat format)
    {
        return Ref<Texture>(new Texture(data, size, format));
    }

    Texture::Texture(const uint8* data, const uvec2& size, TextureFormat format)
      : m_format(format),
        m_size(size)
    {
        DRK_PROFILE_FUNCTION();

        glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
        glTextureStorage2D(m_id, 1, to_gl_internal_format(format), size.x, size.y);

        // Could use builder pattern for parameters

        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_id, 0, 0, 0, size.x, size.y, to_gl_data_format(format), GL_UNSIGNED_BYTE, data);
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
        glBindTextureUnit(slot, m_id);
    }

    void Texture::detach(void) const
    {
        if (m_slot)
            glBindTextureUnit(*m_slot, 0);
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
      : m_maxTextures(get_max_textures()),
        m_numReserved(0)
    {
        DRK_LOG_CORE_TRACE("Max textures: {}", m_maxTextures);
        m_textures.reserve(m_maxTextures);
    }

    uint TextureManager::reserve(void)
    {
        DRK_ASSERT_DEBUG(m_numReserved == count(), "Can only reserve texture slot before adding additional textures");
        DRK_ASSERT_DEBUG(m_numReserved < m_maxTextures, "{} texture slots already reserved", m_numReserved);

        m_textures.push_back({});
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
        DRK_ASSERT_CORE(count() > slot, "Invalid slot: {}", slot);
        texture->attach(slot);
        m_textures[slot] = std::move(texture);
    }

    void TextureManager::attach(Ref<Texture> texture)
    {
        if (full())
        {
            DRK_ASSERT_DEBUG(false, "All texture slots are full");
            DRK_LOG_CORE_ERROR("All texture slots are full");
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
