#include "Util.hpp"

#include "Profiler.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <locale>
#include <cctype>

namespace DrkCraft
{
    std::string read_file(std::filesystem::path path)
    {
        if (std::filesystem::is_regular_file(path))
        {
            std::ifstream file(path);
            std::stringstream contents;
            contents << file.rdbuf();
            return contents.str();
        }
        else
            return "";
    }

    std::string read_file(const std::filesystem::path& filename)
    {
        if (file_exists(filename))
        {
            std::ifstream file(filename);
            std::stringstream contents;
            contents << file.rdbuf();
            return contents.str();
        }
        else
            return "";
    }

    bool file_exists(const std::filesystem::path& filename)
    {
        return std::filesystem::is_regular_file(filename);
    }

    bool dir_exists(const std::filesystem::path& dirname)
    {
        return std::filesystem::is_directory(dirname);
    }

    static std::locale currentLocale;

    std::string capitalize(std::string_view str)
    {
        std::string capitalized(str);
        if (str.size() > 0)
            capitalized[0] = std::toupper(str[0], currentLocale);
        return capitalized;
    }

    std::string to_lower(std::string_view str)
    {
        std::string lower;
        std::ranges::transform(str, lower.begin(), [](char c)
        {
            return std::tolower(c, currentLocale);
        });
        return lower;
    }

    std::string to_upper(std::string_view str)
    {
        std::string upper;
        std::ranges::transform(str, upper.begin(), [](char c)
        {
            return std::toupper(c, currentLocale);
        });
        return upper;
    }

    RandomEngine get_random_engine(void)
    {
        std::random_device rd;
        return RandomEngine(rd());
    }

    RandomIntDist::RandomIntDist(int min, int max)
      : rng(get_random_engine()),
        dist(min, max)
    { }

    int RandomIntDist::get(void)
    {
        return dist(rng);
    }

    int RandomIntDist::operator()(void)
    {
        return get();
    }

    RandomFloatDist::RandomFloatDist(float min, float max)
      : rng(get_random_engine()),
        dist(min, max)
    { }

    float RandomFloatDist::get(void)
    {
        return dist(rng);
    }

    float RandomFloatDist::operator() (void)
    {
        return get();
    }

    IntervalCounter::IntervalCounter(uint interval)
      : m_INTERVAL(interval),
        m_count(0)
    { }

    void IntervalCounter::count(int x)
    {
        m_count += x;
    }

    bool IntervalCounter::on_interval(void)
    {
        if (m_count >= m_INTERVAL)
        {
            m_count = 0;
            return true;
        }
        else
            return false;
    }

    IntervalTimer::IntervalTimer(int interval)
      : m_INTERVAL(interval)
    { }

    bool IntervalTimer::on_interval(void)
    {
        if (m_timer.get_elapsed() >= m_INTERVAL)
        {
            m_timer.reset();
            return true;
        }
        else
            return false;
    }
}
