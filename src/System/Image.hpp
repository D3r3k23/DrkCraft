#ifndef DRK_IMAGE_HPP
#define DRK_IMAGE_HPP

#include "Core/Base.hpp"

#include <stb/stb_image.h>
#include <glm/vec2.hpp>

#include <filesystem>

namespace DrkCraft
{
    class Image
    {
    public:
        Image(const std::filesystem::path& filename);
        ~Image(void);

    private:
        stbi_uc* m_data;
        glm::uvec2 m_size;
        uint m_channels;
    };
}

#endif // DRK_IMAGE_HPP
