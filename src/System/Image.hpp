#ifndef DRK_SYSTEM_IMAGE_HPP
#define DRK_SYSTEM_IMAGE_HPP

#include "Core/Base.hpp"

#include "lib/fs.hpp"
#include "lib/glm/vec2.hpp"

namespace DrkCraft
{
    struct ImageData
    {
        ImageData(uint8* data, const vec2& size, uint channels);
        ~ImageData(void);

        ImageData(ImageData&& other);
        ImageData& operator=(ImageData&& other);

        ImageData(const ImageData&) = delete;
        ImageData& operator=(const ImageData&) = delete;

        void destroy(void);
        operator bool(void) const;
    };

    class Image
    {
    public:
        static Ptr<Image> load_file(const fs::path& filename, uint channels=0);

        Image(uint8* data, const uvec2& size, uint channels);
        ~Image(void);

        uint8* data;
        const uvec2 size;
        const uint channels;
    };
}

#endif // DRK_SYSTEM_IMAGE_HPP
