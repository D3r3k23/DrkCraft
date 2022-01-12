#ifndef DRK_CORE_UTIL_HPP
#define DRK_CORE_UTIL_HPP

#include "Base.hpp"

#include <string>
#include <string_view>
#include <span>
#include <filesystem>
#include <random>
#include <algorithm>
#include <utility>

namespace DrkCraft
{
    std::string read_file(std::filesystem::path path);

    std::string capitalize(std::string_view str);

    std::string to_lower(std::string_view str);
    std::string to_upper(std::string_view str);

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

    template <int SIZE>
    class ByteBuffer
    {
    public:
        ByteBuffer(void)
        {
            m_data = new Byte[SIZE];
        }

        ByteBuffer(std::span<Byte> data)
          : ByteBuffer()
        {
            DRK_ASSERT_DEBUG_NO_MSG(data.size() <= SIZE);
            std::copy(data, data + SIZE, m_data);
        }

        ByteBuffer(const ByteBuffer<SIZE>& other)
          : ByteBuffer()
        {
            std::copy(data, data + SIZE, m_data);
        }

        ByteBuffer& operator=(const ByteBuffer<SIZE>& other)
        {
            std::fill(m_data, m_data + SIZE, 0);
            std::copy(data, data + SIZE, m_data);
            return *this;
        }

        ByteBuffer(ByteBuffer<SIZE>&& other)
        {
            m_data = other.m_data;
            other.m_data = nullptr;
        }

        ByteBuffer& operator=(ByteBuffer<SIZE>&& other)
        {
            m_data = other.m_data;
            return *this;
        }

        ~ByteBuffer(void)
        {
            delete[] m_data;
        }

        operator std::span<Byte>(void) const
            { return { m_data, SIZE }; }

        Byte* data(void)
            { return m_data; }

        uint size(void) const
            { return SIZE; }

    private:
        Byte* m_data;
    };
}

#endif // DRK_CORE_UTIL_HPP
