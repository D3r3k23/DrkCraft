#ifndef DRK_BUFFER_HPP
#define DRK_BUFFER_HPP

#include "Core/Base.hpp"

#include <glad/glad.h>

#include <vector>

namespace DrkCraft
{
    class BufferElement
    {

    };

    using BufferObjectID = GLuint;

    class BufferObject
    {
    public:
        BufferObject(void)
        {
            glGenBuffers(1, &m_id);
        }

        virtual void bind(void) = 0;

    protected:
        BufferObjectID m_id;

        std::vector<BufferElement> m_elements;
    };
}

#endif // DRK_BUFFER_HPP
