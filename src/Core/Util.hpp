#ifndef DRK_CORE_UTIL_HPP
#define DRK_CORE_UTIL_HPP

#include "Base.hpp"
#include "Timer.hpp"

#include <string>
#include <string_view>
#include <span>
#include <filesystem>
#include <random>
#include <functional>
#include <utility>

namespace DrkCraft
{
    std::string read_file(const std::filesystem::path& filename);

    bool file_exists(const std::filesystem::path& filename);
    bool dir_exists(const std::filesystem::path& dirname);

    std::string capitalize(std::string_view str);

    std::string to_lower(std::string_view str);
    std::string to_upper(std::string_view str);

    template <typename R, typename ... Args>
    std::function<R(Args...)> add_prefix_function(std::function<R(Args...)> fn, std::function<void(void)> pre)
    {
        return [&](auto&& ... args) -> R
        {
            pre();
            fn(std::forward<Args>(args)...);
        };
    }

    using RandomEngine = std::mt19937;

    RandomEngine get_random_engine(void);

    class RandomIntDist
    {
    public:
        RandomIntDist(int min, int max);
        int get(void);
        int operator() (void);

    private:
        RandomEngine rng;
        std::uniform_int_distribution<int> dist;
    };

    class RandomFloatDist
    {
    public:
        RandomFloatDist(float min, float max);
        float get(void);
        float operator()(void);

    private:
        RandomEngine rng;
        std::uniform_real_distribution<float> dist;
    };

    class IntervalCounter
    {
    public:
        IntervalCounter(uint interval);

        void count(int x=1);
        bool on_interval(void);

    private:
        const uint m_INTERVAL;
        uint m_count;
    };

    class IntervalTimer
    {
    public:
        IntervalTimer(int interval); // Milliseconds

        bool on_interval(void);

    private:
        const Time::Milli<int> m_INTERVAL;
        Timer m_timer;
    };
}

#endif // DRK_CORE_UTIL_HPP
