#include "VertexArray.hpp"

#include "Core/Debug/Profiler.hpp"

#include <glad/glad.h>

namespace DrkCraft
{
    VertexArray::VertexArray(void)
    {
        glCreateVertexArrays(1, &m_id);
    }
    VertexArray::~VertexArray(void)
    {
        glDeleteVertexArrays(1, &m_id);
    }

    void VertexArray::bind(void) const
    {
        glBindVertexArray(m_id);
    }

    void VertexArray::unbind(void) const
    {
        glBindVertexArray(0);
    }

    void VertexArray::set_vertex_buffer(const Ref<VertexBuffer> buffer)
    {
        DRK_PROFILE_FUNCTION();

        bind();
        buffer->bind();

        buffer->activate_layout();
        m_vertexBuffer = buffer;

        unbind();
        buffer->bind(); // ?
    }

    void VertexArray::set_index_buffer(const Ref<IndexBuffer> buffer)
    {
        m_indexBuffer = buffer;
    }

    const Ref<VertexBuffer>& VertexArray::get_vertex_buffer(void) const
    {
        return m_vertexBuffer;
    }

    const Ref<IndexBuffer>& VertexArray::get_index_buffer(void) const
    {
        return m_indexBuffer;
    }
}
