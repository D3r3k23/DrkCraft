#ifndef DRK_CORE_VERSION_HPP
#define DRK_CORE_VERSION_HPP

#include "Core/Type.hpp"

#include <fmt/core.h>

#include <string>
#include <string_view>
#include <charconv>
#include <compare>

namespace DrkCraft
{
    class Version
    {
    public:
        constexpr Version(std::string_view ver) // ver: "<major>.<minor>"
          : m_str(ver),
            m_major(find_major(ver)),
            m_minor(find_minor(ver))
        { }

        Version(uint major, uint minor)
          : m_str(make_string(major, minor)),
            m_major(major),
            m_minor(minor)
        { }

        Version(const Version&) = default;
        Version& operator=(const Version&) = default;

        constexpr std::string_view string(void) const { return m_str; }

        constexpr uint major(void) const { return m_major; }
        constexpr uint minor(void) const { return m_minor; }

        friend constexpr std::strong_ordering operator<=>(const Version& v1, const Version& v2)
        {
            if (v1.m_major == v2.m_major)
                return v1.m_minor <=> v2.m_minor;
            else
                return v1.m_major <=> v2.m_minor;
        }

        friend constexpr std::strong_ordering operator<=>(const Version& ver, std::string_view str)
            { return ver <=> Version(str); }

        friend constexpr std::strong_ordering operator<=>(std::string_view str, const Version& ver)
            { return ver <=> str; }

    private:
        static std::string make_string(uint major, uint minor)
        {
            return fmt::format("{}.{}", major, minor);
        }

        static constexpr uint find_major(std::string_view ver)
        {
            if (uint sep = ver.find("."); sep != std::string::npos)
            {
                uint major = 0;
                const auto r = std::from_chars(ver.data() + sep + 1, ver.data() + ver.length(), major);
                return major;
            }
            else
                return 0;
        }

        static constexpr uint find_minor(std::string_view ver)
        {
            if (uint sep = ver.find("."); sep != std::string::npos)
            {
                uint minor = 0;
                const auto r = std::from_chars(ver.data() + sep + 1, ver.data() + ver.length(), minor);
                return minor;
            }
            else
                return 0;
        }

    private:
        const std::string m_str;
        const uint m_major;
        const uint m_minor;
    };
}

#endif // DRK_CORE_VERSION_HPP
