#include "GlBuffer.hpp"

#include "Core/Profiler.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace DrkCraft
{
    //////////////////////////
    //       GlBuffer       //
    //////////////////////////

    GlBuffer::GlBuffer(uint size, uint count)
      : m_size(size),
        m_count(count)
    {
        DRK_PROFILE_FUNCTION();
        glCreateBuffers(1, &m_id);
    }

    GlBuffer::~GlBuffer(void)
    {
        glDeleteBuffers(1, &m_id);
    }

    uint GlBuffer::get_size(void) const
    {
        return m_size;
    }

    uint GlBuffer::get_count(void) const
    {
        return m_count;
    }

    //////////////////////////////
    //       VertexBuffer       //
    //////////////////////////////

    VertexBuffer::VertexBuffer(std::span<Vertex> vertices)
      : VertexBuffer(glm::value_ptr(vertices[0]), vertices.size() * 3)
    { }

    VertexBuffer::VertexBuffer(std::span<VertexElement> vertices)
      : VertexBuffer(vertices.data(), vertices.size())
    { }

    VertexBuffer::VertexBuffer(VertexElement* vertices, uint count)
      : GlBuffer(count * sizeof(VertexElement), count)
    {
        DRK_PROFILE_FUNCTION();

        bind();
        glBufferData(GL_ARRAY_BUFFER, get_size(), vertices, GL_STATIC_DRAW);
        unbind();
    }

    void VertexBuffer::bind(void)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void VertexBuffer::unbind(void)
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    /////////////////////////////
    //       IndexBuffer       //
    /////////////////////////////

    IndexBuffer::IndexBuffer(std::span<Index> indices)
      : IndexBuffer(indices.data(), indices.size())
    { }

    IndexBuffer::IndexBuffer(Index* indices, uint count)
      : GlBuffer(count * sizeof(Index), count)
    {
        DRK_PROFILE_FUNCTION();

        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, get_size(), indices, GL_STATIC_DRAW);
        unbind();
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
