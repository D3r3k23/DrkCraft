#ifndef DRK_CORE_VERSION_HPP
#define DRK_CORE_VERSION_HPP

#include "Core/Type.hpp"

#include <string>
#include <compare>

namespace DrkCraft
{
    class Version
    {
    public:
        Version(const std::string& ver); // ver: "<major>.<minor>"
        Version(uint major, uint minor);

        Version(const Version&) = default;
        Version& operator=(const Version&) = default;

        std::string string(void) const;

        uint major(void) const;
        uint minor(void) const;

        friend std::strong_ordering operator<=>(const Version& v1, const Version& v2);
        friend std::strong_ordering operator<=>(const Version& ver, const std::string& str);
        friend std::strong_ordering operator<=>(const std::string& str, const Version& ver);

    private:
        static constexpr uint find_major(const std::string& ver);
        static constexpr uint find_minor(const std::string& ver);

    private:
        const uint m_major;
        const uint m_minor;
    };
}

#endif // DRK_CORE_VERSION_HPP
