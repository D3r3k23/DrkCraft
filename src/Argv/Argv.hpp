#ifndef DRK_UTIL_ARGV_HPP
#define DRK_UTIL_ARGV_HPP

#include <vector>
#include <string_view>
#include <algorithm>
#include <optional>
#include <iterator>

namespace DrkCraft
{
    class Argv // Simple wrapper for C-style argc/argv
    {
        using Container = std::vector<std::string_view>;
        using Iterator  = Container::const_iterator;

    public:
        Argv(int argc, char* argv[])
          : m_argv(argc)
        {
            for (uint i = 0; i < argc; ++i)
                m_argv[i] = argv[i];
        }

        std::string_view operator[](int i) const
        {
            return m_argv[i];
        }

        std::optional<std::string_view> get(int i) const
        {
            if (0 <= i && i < count())
                return m_argv[i];
            else
                return {};
        }

        unsigned count(void) const
        {
            return m_argv.size();
        }

        unsigned find_index(std::string_view arg) const
        {
            return std::distance(begin(), find(arg));
        }

        bool contains(std::string_view arg) const
        {
            return find(arg) != end();
        }

        Iterator find(std::string_view arg) const
        {
            return std::ranges::find(m_argv, arg);
        }

        Iterator begin(void) const
        {
            return m_argv.cbegin();
        }

        Iterator end(void) const
        {
            return m_argv.cend();
        }

    private:
        Container m_argv;
    };
}

#endif // DRK_UTIL_ARGV_HPP
