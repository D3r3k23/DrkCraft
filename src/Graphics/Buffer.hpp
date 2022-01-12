#ifndef DRK_BUFFER_HPP
#define DRK_BUFFER_HPP

#include "Core/Base.hpp"
#include "OpenGlObject.hpp"

#include <glad/glad.h>
#include <glm/vec3.hpp>

#include <vector>
#include <span>

namespace DrkCraft
{
    class BufferElement
    {

    };

    class Buffer : public OpenGlObject
    {
    public:
        Buffer(void);
        virtual ~Buffer(void);

        virtual void bind(void) = 0;
        virtual void unbind(void) = 0;

    protected:
        std::vector<BufferElement> m_elements;
    };

    class VertexBuffer : public Buffer
    {
    public:
        VertexBuffer(std::span<glm::vec3> vertices);
        VertexBuffer(std::span<float> vertices);
        VertexBuffer(float* vertices, uint count);

        virtual void bind(void) override;
        virtual void unbind(void) override;
    };

    using Index = GLuint;

    class IndexBuffer : public Buffer
    {
    public:
        IndexBuffer(std::span<Index> indices);
        IndexBuffer(Index* indices, uint count);

        virtual void bind(void) override;
        virtual void unbind(void) override;
    };
}

#endif // DRK_BUFFER_HPP
