#include "Icon.hpp"

namespace DrkCraft
{
    Icon::Icon(const fs::path& filename)
      : m_image(Image::load_file(filename, 4))
    { }

    bool Icon::valid(void) const
    {
        return static_cast<bool>(m_image);
    }

    uint8* Icon::get_data(void) const
    {
        return m_image->data;
    }

    const uvec2& Icon::get_size(void) const
    {
        return m_image->size;
    }
}
