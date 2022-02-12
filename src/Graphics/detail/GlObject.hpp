#ifndef DRK_GRAPHICS_DETAIL_GL_OBJECT_HPP
#define DRK_GRAPHICS_DETAIL_GL_OBJECT_HPP

#include "Core/Base.hpp"

namespace DrkCraft
{
    using GlObjectID = uint;

    class GlObject
    {
    protected:
        GlObject(void) = default;
        virtual ~GlObject(void) = default;

    public:
        GlObject(const GlObject&) = delete;
        GlObject& operator=(const GlObject&) = delete;

        virtual void bind(void) const { };
        virtual void unbind(void) const { };

        GlObjectID get_id(void) const
        {
            return m_id;
        }

    protected:
        GlObjectID m_id;
    };
}

#endif // DRK_GRAPHICS_DETAIL_GL_OBJECT_HPP
