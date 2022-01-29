#ifndef DRK_IMAGE_HPP
#define DRK_IMAGE_HPP

#include "Core/Base.hpp"

#include <stb/stb_image.h> // Make this internal to implementation?
#include <glm/vec2.hpp>

#include <filesystem>
#include <concepts>

namespace DrkCraft
{
    struct ImageData
    {
        using Data_t = stbi_uc;
        // Ensure that data can be stored as uint8*
        static_assert(std::unsigned_integral<Data_t>);
        static_assert(sizeof(uint8) == sizeof(Data_t));

        ImageData(uint8* data, const glm::vec2& size, uint channels);
        ~ImageData(void);

        uint8* data;
        glm::uvec2 size;
        uint channels;
    };

    class Image
    {
    public:
        static Ptr<ImageData> load(const std::filesystem::path& filename, uint channels=0);

        Image(const std::filesystem::path& filename);

    private:
        Ptr<ImageData> m_data;
    };
}

#endif // DRK_IMAGE_HPP
