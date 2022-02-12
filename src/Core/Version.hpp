#ifndef DRK_CORE_VERSION_HPP
#define DRK_CORE_VERSION_HPP

#include "lib/type.hpp"

#include <string>
#include <compare>

namespace DrkCraft
{
    class Version
    {
    public:
        Version(std::string ver); // ver: "<major>.<minor>"
        Version(uint major, uint minor);

        Version(const Version&) = default;

        std::string string(void) const;

        uint major(void) const;
        uint minor(void) const;

    private:
        const uint m_major;
        const uint m_minor;

    private:
        static uint find_major(std::string ver);
        static uint find_minor(std::string ver);

    public:
        friend std::strong_ordering operator<=>(const Version& v1, const Version& v2);
    };
}

#endif // DRK_CORE_VERSION_HPP
