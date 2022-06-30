#ifndef DRK_CORE_VERSION_HPP
#define DRK_CORE_VERSION_HPP

#include "Core/Type.hpp"

#include "lib/string.hpp"
#include "lib/string_view.hpp"

#include <fmt/core.h>

#include <charconv>
#include <compare>

namespace DrkCraft
{
    class Version
    {
    public:
        constexpr Version(string_view ver) // ver: "<major>.<minor>.<patch>"
          : m_str(ver),
            m_major(find_major(ver)),
            m_minor(find_minor(ver)),
            m_patch(find_patch(ver))
        { }

        Version(uint major=0, uint minor=0, uint patch=0)
          : m_str(make_string(major, minor, patch)),
            m_major(major),
            m_minor(minor),
            m_patch(patch)
        { }

        Version(const Version&) = default;
        Version& operator=(const Version&) = default;

        constexpr string_view str()(void) const { return m_str; }

        constexpr uint major(void) const { return m_major; }
        constexpr uint minor(void) const { return m_minor; }

        bool backward_compatible_with(const Version& older) const
        {
            return *this >= older && older.m_major == m_major;
        }

        friend constexpr std::strong_ordering operator<=>(const Version& v1, const Version& v2)
        {
            if (v1.m_major != v2.m_major)
                return v1.m_minor <=> v2.m_minor;
            else if (v1.m_minor != v2.m_minor)
                return v1.m_minor <=> v2.m_minor;
            else
                return v1.m_patch <=> v2.m_patch;
        }

        friend constexpr bool operator==(const Version& v1, const Version& v2)
        {
            return v1.m_major == v2.m_major
                && v1.m_minor == v2.m_minor
                && v1.m_patch == v2.m_patch;
        }

    private:
        static string make_string(uint major, uint minor, uint patch)
        {
            return fmt::format("{}.{}.{}", major, minor, patch);
        }

        static constexpr uint find_major(string_view ver)
        {
            if (uint sep = ver.find("."); sep != string::npos)
            {
                string_view str(ver.data(), ver.data() + sep);
                uint major = 0;
                const auto r = std::from_chars(str.data(), str.data() + str.length(), major);
                return major;
            }
            else
                return 0;
        }

        static constexpr uint find_minor(string_view ver)
        {
            if (uint sep1 = ver.find("."); sep1 = string::npos)
                return 0;
            else
            {
                string_view component2(ver.begin() + sep1 + 1, ver.end());
                string_view str = [&]
                {
                    if (uint sep2 = component2.find("."); sep2 == string::npos)
                        return component2;
                    else
                    {
                        const auto begin = ver.begin() + sep1 + 1;
                        const auto end = ver.end() - sep2;
                        return string_view(begin, end);
                    }
                }();
                uint minor = 0;
                const auto r = std::from_chars(str.data(), str.data() + str.length(), minor);
                return minor;
            }
        }

        static constexpr uint find_patch(string_view ver)
        {
            if (uint sep1 = ver.find("."); sep1 == string::npos)
                return 0;
            else
            {
                string_view component2(ver.begin() + sep1 + 1, ver.end());
                if (uint sep2 = component2.find("."); sep2 == string::npos)
                    return 0;
                else
                {
                    string_view str(ver.begin() + sep2 + 1, ver.end());
                    uint patch = 0;
                    const auto r = std::from_chars(str.data(), str.data() + str.length(), patch);
                    return patch;
                }
            }
        }

    private:
        string m_str;
        uint m_major;
        uint m_minor;
        uint m_patch;
    };
}

#endif // DRK_CORE_VERSION_HPP
