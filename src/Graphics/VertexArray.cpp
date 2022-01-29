#include "VertexArray.hpp"
#include "Core/Profiler.hpp"

#include <glad/glad.h>

namespace DrkCraft
{
    VertexArray::VertexArray(void)
    {
        glGenVertexArrays(1, &m_id);
    }
    VertexArray::~VertexArray(void)
    {
        glDeleteVertexArrays(1, &m_id);
    }

    void VertexArray::bind(void) const
    {
        glBindVertexArray(m_id);
    }

    void VertexArray::bind(void) const
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
    }

    void VertexArray::set_index_buffer(const Ref<IndexBuffer> buffer)
    {

    }
}
