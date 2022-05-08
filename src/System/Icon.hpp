#ifndef DRK_SYSTEM_ICON_HPP
#define DRK_SYSTEM_ICON_HPP

#include "Core/Base.hpp"
#include "System/Image.hpp"

#include "lib/fs.hpp"
#include "lib/glm/vec2.hpp"

namespace DrkCraft
{
    class Icon
    {
    public:
        Icon(const fs::path& filename);

        bool valid(void) const;

        const uint8* get_data(void) const;
        const uvec2& get_size(void) const;

    private:
        const Ptr<Image> m_image;
    };
}

#endif // DRK_SYSTEM_ICON_HPP
