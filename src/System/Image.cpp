#include "Image.hpp"

#include "Core/Profiler.hpp"

namespace DrkCraft
{
    ImageData::ImageData(uint8* data, const glm::vec2& size, uint channels)
      : data(data), size(size), channels(channels)
    { }

    ImageData::~ImageData(void)
    {
        DRK_PROFILE_FUNCTION();
        stbi_image_free(data);
    }

    Ptr<ImageData> Image::load(const std::filesystem::path& filename, uint channels)
    {
        DRK_PROFILE_FUNCTION();

        int channelsOut;
        int* channelsPtr;
        if (channels == 0)
        {
            channels = 4;
            channelsPtr = nullptr;
        }
        else
            channelsPtr = &channelsOut;

        ImageData::Data_t* data;
        glm::uvec2 size;
        {
            DRK_PROFILE_SCOPE("stbi_load");
            int width, height;
            data = stbi_load(filename.string().c_str(), &width, &height, channelsPtr, channels);
            size = { width, height };
            if (channelsPtr)
                channels = channelsOut;
        }
        return make_ptr<ImageData>(static_cast<uint8*>(data), size, channels);
    }

    Image::Image(const std::filesystem::path& filename)
      : m_data(load(filename))
    {
        DRK_PROFILE_FUNCTION();
    }
}
