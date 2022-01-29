#ifndef DRK_ICON_HPP
#define DRK_ICON_HPP

#include "Core/Base.hpp"
#include "Image.hpp"

#include <glm/vec2.hpp>

namespace DrkCraft
{
    class Icon
    {
    public:
        Icon(const std::filesystem::path& filename);
        ~Icon(void);

        uint8* get_data(void) const;
        const glm::uvec2& get_size(void) const;

    private:
        Ptr<ImageData> m_image;
    };
}

#endif // DRK_ICON_HPP
