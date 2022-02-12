#ifndef DRK_GRAPHICS_DETAIL_GL_TOOLS_HPP
#define DRK_GRAPHICS_DETAIL_GL_TOOLS_HPP

#include "Core/Base.hpp"
#include "Graphics/detail/GlObject.hpp"

#include <concepts>

namespace DrkCraft
{
    void load_opengl(void);

    void GLAPIENTRY gl_message_handler(GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length, const GLchar* msg, const void* userParam);

    template <typename O>
    class GlObjectHandler
    {
    public:
        GlObjectHandler(O& object) requires std::is_derived_from<O, GlObject>
          : m_object(object)
        {
            m_object.bind();
        }

        ~GlObjectHandler(void)
        {
            m_object.unbind();
        }

        O& get(void)
        {
            return m_object;
        }

        GlObjectID id(void) const
        {
            return m_object.get_id();
        }

    private:
        O& m_object;
    };
}

#endif // DRK_GRAPHICS_DETAIL_GL_TOOLS_HPP
