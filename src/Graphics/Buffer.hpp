#ifndef DRK_BUFFER_HPP
#define DRK_BUFFER_HPP

#include "Core/Base.hpp"
#include "GlObject.hpp"
#include "Shader.hpp"

#include <glm/vec3.hpp>

#include <string>
#include <string_view>
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
        uint count;
        uint offset;
        bool normalized;

        VertexAttribute(std::string_view name, ShaderDataType type, bool normalized=false);
    };

    class VertexBufferLayout
    {
    public:
        using Iterator       = std::vector<VertexAttribute>::iterator;
        using const_Iterator = std::vector<VertexAttribute>::const_iterator;

        VertexBufferLayout(void);
        VertexBufferLayout(std::span<VertexAttribute> attributes);
        VertexBufferLayout(std::initializer_list<VertexAttribute> attributes);

        void add_attribute(const VertexAttribute& attribute);
        void add_attribute(std::span<VertexAttribute> attributes);

        void activate(void);

        Iterator begin(void) { return m_attributes.begin(); }
        Iterator end(void)   { return m_attributes.end(); }

        const_Iterator begin(void) const { return m_attributes.begin(); }
        const_Iterator end(void)   const { return m_attributes.end(); }

    private:
        void calc_offsets_and_stride(void);

    private:
        std::vector<VertexAttribute> m_attributes;
        uint m_stride;
    };

    class VertexBuffer : public GlBuffer
    {
    public:
        VertexBuffer(uint size);
        VertexBuffer(const VertexBufferLayout& layout, uint size);
        VertexBuffer(const VertexBufferLayout& layout, void* data, uint count, uint size);

        template <typename T>
        VertexBuffer(const VertexBufferLayout& layout, std::span<T> data)
          : VertexBuffer(layout, static_cast<void*>(data.data()), data.size(), data.size_bytes())
        { }

        bool has_layout(void) const;
        bool has_data(void) const;

        void set_layout(const VertexBufferLayout& layout);
        void activate_layout(void);

        void set_data(void* data, uint size);

        template <typename T>
        void set_data(std::span<T> data)
        {
            set_data(static_cast<void*>(data.data()), data.size());
        }

        virtual void bind(void) const override;
        virtual void unbind(void) const override;

    private:
        VertexBufferLayout m_layout;
    };

    using Index = uint;

    class IndexBuffer : public GlBuffer
    {
    public:
        IndexBuffer(uint size);
        IndexBuffer(std::span<Index> indices);
        IndexBuffer(Index* indices, uint count);

        void set_data(Index* data, uint count);
        void set_data(std::span<Index> data);

        virtual void bind(void) const override;
        virtual void unbind(void) const override;
    };
}

#endif // DRK_BUFFER_HPP
