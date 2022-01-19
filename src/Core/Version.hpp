#ifndef DRK_VERSION_HPP
#define DRK_VERSION_HPP

#include <string>

namespace DrkCraft
{
    class Version
    {
    public:
        Version(std::string ver); // ver: "<major>.<minor>"
        Version(unsigned int major, unsigned int minor);

        Version(const Version&) = default;

        std::string string(void) const;

        unsigned int major(void) const;
        unsigned int minor(void) const;

    private:
        const unsigned int m_major;
        const unsigned int m_minor;

    private:
        static unsigned int find_major(std::string ver);
        static unsigned int find_minor(std::string ver);

    public:
        friend bool operator==(const Version& v1, const Version& v2);
        friend bool operator!=(const Version& v1, const Version& v2);

        friend bool operator<(const Version& v1, const Version& v2);
        friend bool operator>(const Version& v1, const Version& v2);

        friend bool operator<=(const Version& v1, const Version& v2);
        friend bool operator>=(const Version& v1, const Version& v2);
    };
}

#endif // DRK_VERSION_HPP
