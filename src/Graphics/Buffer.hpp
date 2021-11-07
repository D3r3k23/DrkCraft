#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <glad/glad.h>

#include <vector>

namespace DrkCraft
{
    class BufferElement
    {

    };

    class BufferObject
    {
    public:
        BufferObject(void)
        {
            glGenBuffers(1, &m_id);
        }

        virtual void bind(void) = 0;

    protected:
        GLuint m_id;

        std::vector<BufferElement> m_elements;
    };
}

#endif // BUFFER_HPP
