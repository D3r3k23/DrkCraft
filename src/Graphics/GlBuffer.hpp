#ifndef DRK_GL_BUFFER_HPP
#define DRK_GL_BUFFER_HPP

#include "Core/Base.hpp"
#include "GlObject.hpp"

#include <glm/vec3.hpp>

#include <vector>
#include <span>

namespace DrkCraft
{
    class GlBufferElement
    {

    };

    class GlBuffer : public GlObject
    {
    public:
        GlBuffer(uint size, uint count);
        virtual ~GlBuffer(void);

        virtual void bind(void) = 0;
        virtual void unbind(void) = 0;

        uint get_size(void) const;
        uint get_count(void) const;

    protected:
        std::vector<GlBufferElement> m_elements;
        uint m_size;
        uint m_count;
    };

    using Vertex = glm::vec3;
    using VertexElement = float;

    class VertexBuffer : public GlBuffer
    {
    public:
        VertexBuffer(std::span<Vertex> vertices);
        VertexBuffer(std::span<VertexElement> vertices);
        VertexBuffer(VertexElement* vertices, uint count);

        virtual void bind(void) override;
        virtual void unbind(void) override;
    };

    using Index = uint;

    class IndexBuffer : public GlBuffer
    {
    public:
        IndexBuffer(std::span<Index> indices);
        IndexBuffer(Index* indices, uint count);

        virtual void bind(void) override;
        virtual void unbind(void) override;
    };
}

#endif // DRK_GL_BUFFER_HPP
