#ifndef DRK_BYTE_BUFFER_HPP
#define DRK_BYTE_BUFFER_HPP

#include "Core/Base.hpp"

#include <algorithm>

namespace DrkCraft
{
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

#endif // DRK_BYTE_BUFFER_HPP
