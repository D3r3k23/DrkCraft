#ifndef DRK_GRAPHICS_TEXTURE_HPP
#define DRK_GRAPHICS_TEXTURE_HPP

#include "Core/Base.hpp"
#include "Graphics/detail/GlObject.hpp"
#include "Disk/Image.hpp"

#include "Lib/glm/vec2.hpp"

#include <vector>

namespace DrkCraft
{
    enum class TextureFormat
    {
        None = 0,

        RGB,
        RGBA
    };

    class Texture : public GlObject
    {
    public:
        static Ref<Texture> from_image(const Image& image);
        static Ref<Texture> from_data(const uint8* data, const uvec2& size, TextureFormat format);

    private:
        Texture(const uint8* data, const uvec2& size, TextureFormat format);

    public:
        virtual ~Texture(void);

        void attach(uint slot) const;
        void detach(void) const;

        bool attached(void) const;

        uvec2 get_coordinates(uint s, uint t);

    private:
        mutable Optional<uint> m_slot;

        uvec2 m_size;
        TextureFormat m_format;
    };

    class TextureAtlas
    {
    public:
        TextureAtlas(Ref<Texture> atlasTexture, uint numTextures);

        Ref<Texture> get_texture(void) const;

        uvec2 get_coordinates(uint index, const ivec2& subtextureCoordinates);

    private:
        Ref<Texture> m_texture;
        const uint m_numTextures;
    };

    class TextureManager
    {
    public:
        TextureManager(void);

        uint reserve(void);

        void refresh(void);

        void attach(Ref<Texture> texture, uint slot);
        void attach(Ref<Texture> texture);

        uint count(void) const;
        bool full(void) const;
        bool empty(void) const;

    private:
        static uint get_max_textures(void);

    private:
        const uint m_maxTextures;
        std::vector<Ref<Texture>> m_textures;

        uint m_numReserved;
    };
}

#endif // DRK_GRAPHICS_TEXTURE_HPP
