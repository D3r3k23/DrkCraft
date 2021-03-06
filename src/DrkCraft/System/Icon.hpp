#ifndef DRK_SYSTEM_ICON_HPP
#define DRK_SYSTEM_ICON_HPP

#include "Core/Base.hpp"
#include "Disk/Image.hpp"

#include "Lib/fs.hpp"
#include "Lib/glm/vec2.hpp"

namespace DrkCraft
{
    class Icon
    {
    public:
        Icon(const fs::path& filename);

        bool valid(void) const;

        const uint8* get_image_data(void) const;

        const uvec2& get_size(void) const;

    private:
        const Ptr<Image> m_image;
    };
}

#endif // DRK_SYSTEM_ICON_HPP
