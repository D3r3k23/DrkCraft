#ifndef DRK_GRAPHICS_TEXTURE_HPP
#define DRK_GRAPHICS_TEXTURE_HPP

#include "Core/Base.hpp"
#include "Graphics/detail/GlObject.hpp"

#include "lib/glm/vec2.hpp"

#include <vector>

namespace DrkCraft
{
    class Texture : public GlObject
    {
    public:
        Texture(void) = default;
        virtual ~Texture(void);

        virtual void bind(void) const = 0;
        virtual void unbind(void) const = 0;

        virtual uvec2 get_coordinates(uint s, uint t);

    protected:

    private:
    };

    class Texture2D : public Texture
    {
    public:
        Texture2D(void);
        virtual ~Texture2D(void);

        virtual void bind(void) const override;
        virtual void unbind(void) const override;

    protected:

    private:
    };

    class TextureAtlas : public Texture2D
    {
    public:
        TextureAtlas(void);

        virtual uvec2 get_coordinates(uint s, uint t) override;

    private:
    };

    class TextureManager
    {
    public:
        TextureManager(void);

    private:
        const uint m_MAX_TEXTURES;
        std::vector<uint> m_textures;

    private:
        static uint get_max_textures(void);
    };
}

#endif // DRK_GRAPHICS_TEXTURE_HPP
