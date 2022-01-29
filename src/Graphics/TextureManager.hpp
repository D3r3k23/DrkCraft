#ifndef DRK_TEXTURE_MANAGER_HPP
#define DRK_TEXTURE_MANAGER_HPP

#include "Core/Base.hpp"

#include <vector>

namespace DrkCraft
{
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

#endif // DRK_TEXTURE_MANAGER_HPP
