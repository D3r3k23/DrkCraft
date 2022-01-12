#ifndef DRK_OPEN_GL_OBJECT_HPP
#define DRK_OPEN_GL_OBJECT_HPP

#include "Core/Base.hpp"

#include <concepts>

namespace DrkCraft
{
    using GlObjectID = uint;

    template <typename O>
    concept OpenGlObjectConcept = requires(O object)
    {
        { object.get_id() } -> std::convertible_to<GlObjectID>;
        object.bind();
        object.unbind();
    };

    class OpenGlObject
    {
    public:
        OpenGlObject(void) = default;
        virtual ~OpenGlObject(void) = default;

        virtual void bind(void) { };
        virtual void unbind(void) { };

        GlObjectID get_id(void) const;

    protected:
        GlObjectID m_id;
    };
}

#endif // DRK_OPEN_GL_OBJECT_HPP
