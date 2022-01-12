#include "Image.hpp"

namespace DrkCraft
{
    Image::Image(const std::filesystem::path& filename)
    {
        int width, height, channels;
        m_data = stbi_load(filename.string().c_str(), &width, &height, &channels, 0);
        m_size = { width, height };
        m_channels = channels;
    }

    Image::~Image(void)
    {
        stbi_image_free(m_data);
    }
}
