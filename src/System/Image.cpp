#include "Image.hpp"

#include "Core/Debug/Profiler.hpp"

#include <stb/stb_image.h>

#include <concepts>

namespace DrkCraft
{
    ///////////////////////
    //       Image       //
    ///////////////////////

    using Data_t = stbi_uc;
    // Ensure that data can be stored as uint8*
    static_assert(std::unsigned_integral<Data_t>);
    static_assert(sizeof(uint8) == sizeof(Data_t));

    Ptr<Image> Image::load_file(const fs::path& filename, uint channels)
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

        uint8* data;
        uvec2 size;
        {
            DRK_PROFILE_SCOPE("stbi_load");
            int width, height;
            data = (uint8*)stbi_load(filename.string().c_str(), &width, &height, channelsPtr, channels);

            size = { width, height };
            if (channels == 0)
                channels = channelsOut;
        }
        if (!data)
        {
            DRK_ASSERT_DEBUG(false, "Failed to load image file \"{}\"", filename.generic_string());
            return {};
        }
        return make_ptr<Image>(static_cast<uint8*>(data), size, channels);
    }

    Image::Image(uint8* data, const uvec2& size, uint channels)
      : data(data),
        size(size),
        channels(channels)
    { }

    Image::~Image(void)
    {
        stbi_image_free(data);
    }
}
