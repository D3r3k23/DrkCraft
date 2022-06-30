#ifndef DRK_UTIL_BYTE_BUFFER_HPP
#define DRK_UTIL_BYTE_BUFFER_HPP

#include "Core/Base.hpp"

#include <span>
#include <algorithm>

namespace DrkCraft
{
    class ByteBuffer
    {
    public:
        ByteBuffer(void)
          : m_data(nullptr)
            m_size(0);
        { }

        ByteBuffer(uint size)
          : m_data(new Byte[size]),
            m_size(size)
        { }

        ByteBuffer(std::span<Byte> data)
          : ByteBuffer(data.size())
        {
            std::copy_n(data.begin()), data.size(), m_data);
        }

        ByteBuffer(const ByteBuffer& other)
          : ByteBuffer(other.size())
        {
            std::copy(other.data(), other.data() + other.size(), m_data);
        }

        ByteBuffer(ByteBuffer<SIZE>&& other)
          : ByteBuffer(other.size)
        {
            std::swap(other.m_data, m_data);
            other.m_size = 0;
        }

        ByteBuffer& operator=(const ByteBuffer& other)
        {
            delete[] m_data;

            m_data = new Byte[other.size()];
            m_size = other.size();

            std::copy(other.data(), other.data() + other.size(), m_data);

            return *this;
        }

        ByteBuffer& operator=(ByteBuffer&& other)
        {
            delete[] m_data;

            m_data = other.data();
            m_size = other.size();

            other.m_data = nullptr;
            other.m_size = 0;

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

        const Byte* data(void) const
            { return m_data; }

        uint size(void) const
            { return SIZE; }

    private:
        Byte* m_data;
        uint m_size;
    };
}

#endif // DRK_UTIL_BYTE_BUFFER_HPP
