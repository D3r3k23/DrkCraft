#ifndef DRK_BUFFER_HPP
#define DRK_BUFFER_HPP

#include "Core/Base.hpp"

#include <glad/glad.h>
#include <glm/vec3.hpp>

#include <vector>

namespace DrkCraft
{
    class BufferElement
    {

    };

    using BufferID = GLuint;

    class Buffer
    {
    public:
        ~Buffer(void);

        virtual void bind(void) = 0;
        virtual void unbind(void) = 0;

    protected:
        BufferID m_id;

        std::vector<BufferElement> m_elements;
    };

    class VertexBuffer : public Buffer
    {
    public:
        VertexBuffer(float* vertices, uint count);

        void bind(void) override;
        void unbind(void) override;
    };

    using Index = GLuint;

    class IndexBuffer : public Buffer
    {
    public:
        IndexBuffer(Index* indices, uint count);

        void bind(void) override;
        void unbind(void) override;
    };
}

#endif // DRK_BUFFER_HPP
