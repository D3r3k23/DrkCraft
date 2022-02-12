#ifndef DRK_SYSTEM_IMAGE_HPP
#define DRK_SYSTEM_IMAGE_HPP

#include "Core/Base.hpp"

#include "lib/fs.hpp"
#include "lib/glm/vec2.hpp"

#include <stb/stb_image.h>

#include <concepts>

namespace DrkCraft
{
    struct ImageData
    {
        using Data_t = stbi_uc;
        // Ensure that data can be stored as uint8*
        static_assert(std::unsigned_integral<Data_t>);
        static_assert(sizeof(uint8) == sizeof(Data_t));

        ImageData(uint8* data, const vec2& size, uint channels);
        ~ImageData(void);

        uint8* data;
        uvec2 size;
        uint channels;
    };

    class Image
    {
    public:
        static Ref<ImageData> load(const fs::path& filename, uint channels=0);

        Image(const fs::path& filename);

    private:
        Ptr<ImageData> m_data;
    };
}

#endif // DRK_SYSTEM_IMAGE_HPP
