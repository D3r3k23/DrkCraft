#include "Version.hpp"

#include <fmt/format.h>

namespace DrkCraft
{
    Version::Version(std::string ver) // ver: "<major>.<minor>"
      : m_major(find_major(ver)),
        m_minor(find_minor(ver))
    { }

    Version::Version(uint major, uint minor)
      : m_major(major),
        m_minor(minor)
    { }

    std::string Version::string(void) const
    {
        return fmt::format("{}.{}", m_major, m_minor);
    }

    uint Version::major(void) const
    {
        return m_major;
    }

    uint Version::minor(void) const
    {
        return m_minor;
    }

    uint Version::find_major(std::string ver)
    {
        if (uint sep = ver.find("."); sep != std::string::npos)
            return std::stoul(ver.substr(0, sep));
        else
            return 0;
    }

    uint Version::find_minor(std::string ver)
    {
        if (uint sep = ver.find("."); sep != std::string::npos)
            return std::stoul(ver.substr(sep + 1, ver.length() - sep - 1));
        else
            return 0;
    }

    std::strong_ordering operator<=>(const Version& v1, const Version& v2)
    {
        if (v1.m_major == v2.m_major)
            return v1.m_minor <=> v2.m_minor;
        else
            return v1.m_major <=> v2.m_minor;
    }
}
