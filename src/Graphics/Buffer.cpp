#include "Buffer.hpp"

#include "Core/Profiler.hpp"

namespace DrkCraft
{
    Buffer::~Buffer(void)
    {
        glDeleteBuffers(1, &m_id);
    }

    VertexBuffer::VertexBuffer(float* vertices, uint count)
    {
        DRK_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), vertices, GL_STATIC_DRAW);
    }

    void VertexBuffer::bind(void)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void VertexBuffer::unbind(void)
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    IndexBuffer::IndexBuffer(Index* indices, uint count)
    {
        DRK_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(Index), indices, GL_STATIC_DRAW);
    }

    void IndexBuffer::bind(void)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void IndexBuffer::unbind(void)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
