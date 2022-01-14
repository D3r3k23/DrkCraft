#ifndef DRK_GRAPHICS_UTIL_HPP
#define DRK_GRAPHICS_UTIL_HPP

#include "Core/Base.hpp"
#include "GlObject.hpp"

namespace DrkCraft
{
    struct OpenGlLoader
    {
        OpenGlLoader(void);
        void load_gl(void);
    };

    template <GlObjectConcept O>
    class GlObjectHandler
    {
    public:
        GlObjectHandler(O& object) : m_object(object)
            {  m_object.bind(); }

        ~GlObjectHandler(void)
            { m_object.unbind(); }

        O& get(void)
            { return m_object; }

        GlObjectID id(void) const
            { return m_object.get_id(); }

    private:
        O& m_object;
    };
}

#endif // DRK_GRAPHICS_UTIL_HPP
