#ifndef DRK_VERTEX_ARRAY_HPP
#define DRK_VERTEX_ARRAY_HPP

#include "Core/Base.hpp"
#include "GlObject.hpp"
#include "Buffer.hpp"

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

    private:
        Ref<VertexBuffer> m_vertexBuffer;
        Ref<IndexBuffer> m_indexBuffer;
    };
}

#endif // DRK_VERTEX_ARRAY
