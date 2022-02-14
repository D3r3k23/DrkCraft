#include "Image.hpp"

#include "Core/Debug/Profiler.hpp"

#include <stb/stb_image.h>

#include <concepts>

namespace DrkCraft
{
    using Data_t = stbi_uc;
    // Ensure that data can be stored as uint8*
    static_assert(std::unsigned_integral<Data_t>);
    static_assert(sizeof(uint8) == sizeof(Data_t));

    ImageData::ImageData(uint8* data, const vec2& size, uint channels)
      : data(data), size(size), channels(channels)
    { }

    ImageData::~ImageData(void)
    {
        DRK_PROFILE_FUNCTION();
        stbi_image_free(data);
    }

    Ref<ImageData> Image::load_file(const fs::path& filename, uint channels)
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
        uvec2 size;
        {
            DRK_PROFILE_SCOPE("stbi_load");
            int width, height;
            data = stbi_load(filename.string().c_str(), &width, &height, channelsPtr, channels);
            size = { width, height };
            if (channelsPtr)
                channels = channelsOut;
        }
        return make_ref<ImageData>(static_cast<uint8*>(data), size, channels);
    }

    Image::Image(const fs::path& filename)
      : m_data(load(filename))
    {
        DRK_PROFILE_FUNCTION();
    }
}
