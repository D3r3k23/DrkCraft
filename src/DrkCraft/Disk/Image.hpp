#ifndef DRK_UTIL_IMAGE_HPP
#define DRK_UTIL_IMAGE_HPP

#include "Core/Base.hpp"

#include "Lib/fs.hpp"
#include "Lib/glm/vec2.hpp"

namespace DrkCraft
{
    class Image
    {
    public:
        static Ptr<Image> load_file(const fs::path& filename, uint channels=0);

        Image(uint8* data, const uvec2& size, uint channels);
        ~Image(void);

        uint8* data(void);
        const uint8* data(void) const;

        const uvec2& size(void) const;
        const uint channels(void) const;

    private:
        uint8* m_data;
        uvec2 m_size;
        uint m_channels;
    };
}

#endif // DRK_UTIL_IMAGE_HPP
