#ifndef DRK_GRAPHICS_DETAIL_BUFFER_HPP
#define DRK_GRAPHICS_DETAIL_BUFFER_HPP

#include "Core/Base.hpp"
#include "Graphics/detail/GlObject.hpp"
#include "Graphics/detail/Util.hpp"

#include <string>
#include <string_view>
#include <optional>
#include <initializer_list>
#include <vector>
#include <span>

namespace DrkCraft
{
    class GlBuffer : public GlObject
    {
    public:
        GlBuffer(void);
        GlBuffer(uint size=0, uint count=0);
        virtual ~GlBuffer(void);

        virtual void bind(void) const = 0;
        virtual void unbind(void) const = 0;

        uint get_size(void) const;
        uint get_count(void) const;

    protected:
        uint m_size;
        uint m_count;
    };

    struct VertexAttribute
    {
        std::string name;
        ShaderDataType type;
        uint size;
        uint offset;
        bool normalized;

        VertexAttribute(ShaderDataType type, std::string_view name, bool normalized=false);
    };

    class VertexBufferLayout
    {
    public:
        using Iterator       = std::vector<VertexAttribute>::iterator;
        using const_Iterator = std::vector<VertexAttribute>::const_iterator;

        VertexBufferLayout(void);
        VertexBufferLayout(std::initializer_list<VertexAttribute> attributes);
        VertexBufferLayout(std::span<VertexAttribute> attributes);

        void add_attribute(const VertexAttribute& attribute);
        uint num_attributes(void) const;

        void activate(void);

        Iterator begin(void) { return m_attributes.begin(); }
        Iterator end(void)   { return m_attributes.end(); }

        const_Iterator begin(void) const { return m_attributes.begin(); }
        const_Iterator end(void)   const { return m_attributes.end(); }

    private:
        void calculate_offsets_and_stride(void);

    private:
        std::vector<VertexAttribute> m_attributes;
        uint m_stride;
    };

    class VertexBuffer : public GlBuffer
    {
    public:
        VertexBuffer(uint size, PrimitiveType type, VertexBufferLayout layout={});
        VertexBuffer(uint size, PrimitiveType type, const VertexBufferLayout& layout, void* data, uint count);

        template <typename T>
        VertexBuffer(const VertexBufferLayout& layout, std::span<T> data)
          : VertexBuffer(layout, static_cast<void*>(data.data()), data.size(), data.size_bytes())
        { }

        void set_layout(const VertexBufferLayout& layout);
        bool has_layout(void) const;
        void activate_layout(void);

        void update(void* data, uint size);

        template <typename T>
        void update(std::span<T> data)
        {
            update(static_cast<void*>(data.data()), data.size());
        }

        PrimitiveType get_primitive_type(void) const;

        virtual void bind(void) const override;
        virtual void unbind(void) const override;

    private:
        VertexBufferLayout m_layout;
        PrimitiveType m_primitiveType;
    };

    using Index = uint;

    class IndexBuffer : public GlBuffer
    {
    public:
        IndexBuffer(uint size);
        IndexBuffer(Index* indices, uint count);
        IndexBuffer(std::span<Index> indices);

        void update(Index* indices, uint count);
        void update(std::span<Index> data);

        virtual void bind(void) const override;
        virtual void unbind(void) const override;

        static uint calculate_size(uint count);
    };
}

#endif // DRK_GRAPHICS_DETAIL_BUFFER_HPP
