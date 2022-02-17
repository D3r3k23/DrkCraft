#ifndef DRK_GRAPHICS_TEXTURE_HPP
#define DRK_GRAPHICS_TEXTURE_HPP

#include "Core/Base.hpp"
#include "Graphics/detail/GlObject.hpp"
#include "System/Image.hpp"

#include "lib/glm/vec2.hpp"

#include <vector>
#include <optional>

namespace DrkCraft
{
    class Texture : public GlObject
    {
    public:
        static Ref<Texture> from_image(const Image& image);
        static Ref<Texture> from_data();

    private:
        Texture(void);
    public:
        virtual ~Texture(void);

        void attach(uint unit) const;
        void detach(void) const;

        bool attached(void) const;

        uvec2 get_coordinates(uint s, uint t);

    private:
        mutable std::optional<uint> m_slot;
    };

    class TextureAtlas
    {
    public:
        TextureAtlas(Ref<Texture> atlasTexture);

        Ref<Texture> get_texture(void) const;

    private:
        Ref<Texture> m_texture;
    };

    class SubTexture
    {
    public:
        SubTexture(Ref<TextureAtlas> atlas, const uvec2& tile);

        uvec2 get_coordinates(uint s, uint t);
    };

    class TextureManager
    {
    public:
        TextureManager(void);

        uint reserve(void);

        void refresh(void);

        void emplace(uint slot, Ref<Texture> texture);

        void push(Ref<Texture> texture);

        uint count(void) const;
        bool full(void) const;
        bool empty(void) const;

    private:
        static uint get_max_textures(void);

    private:
        const uint m_maxTextures;
        std::vector<Ref<Texture>> m_textures;

        uint m_nReserved;
    };
}

#endif // DRK_GRAPHICS_TEXTURE_HPP
