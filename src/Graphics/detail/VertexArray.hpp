#ifndef DRK_GRAPHICS_DETAIL_VERTEX_ARRAY_HPP
#define DRK_GRAPHICS_DETAIL_VERTEX_ARRAY_HPP

#include "Core/Base.hpp"
#include "Graphics/detail/GlObject.hpp"
#include "Graphics/detail/Buffer.hpp"

namespace DrkCraft
{
    class VertexArray : public GlObject
    {
    public:
        VertexArray(void);
        virtual ~VertexArray(void);

        virtual void bind(void) const override;
        virtual void bind(void) const override;

        void set_vertex_buffer(const Ref<VertexBuffer> buffer);
        void set_index_buffer(const Ref<IndexBuffer> buffer);

        const Ref<VertexBuffer>& get_vertex_buffer(void) const;
        const Ref<IndexBuffer>& get_index_buffer(void) const;

    private:
        Ref<VertexBuffer> m_vertexBuffer;
        Ref<IndexBuffer> m_indexBuffer;
    };
}

#endif // DRK_GRAPHICS_DETAIL_VERTEX_ARRAY
