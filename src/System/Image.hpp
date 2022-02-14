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

        uint8* data;
        uvec2 size;
        uint channels;
    };

    class Image
    {
    public:
        static Ref<Image> load_file(const fs::path& filename, uint channels=0);

        Image(ImageData);
        Image(const fs::path& filename);

    private:
        Ptr<ImageData> m_data;
    };
}

#endif // DRK_SYSTEM_IMAGE_HPP
