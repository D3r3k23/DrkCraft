#include "Buffer.hpp"

#include "Core/Debug/Profiler.hpp"

#include <glad/glad.h>

#include <algorithm>

namespace DrkCraft
{
    //////////////////////////
    //       GlBuffer       //
    //////////////////////////

    GlBuffer::GlBuffer(uint size, uint count)
      : m_size(size),
        m_count(count)
    {
        DRK_PROFILE_FUNCTION();
        glCreateBuffers(1, &m_id);
    }

    GlBuffer::~GlBuffer(void)
    {
        DRK_PROFILE_FUNCTION();
        glDeleteBuffers(1, &m_id);
    }

    uint GlBuffer::get_size(void) const
    {
        return m_size;
    }

    uint GlBuffer::get_count(void) const
    {
        return m_count;
    }

    /////////////////////////////////
    //       VertexAttribute       //
    /////////////////////////////////

    VertexAttribute::VertexAttribute(ShaderDataType type, std::string_view name, bool normalized)
      : name(name),
        type(type),
        size(get_shader_data_type_size(type)),
        offset(0),
        normalized(normalized)
    { }

    ////////////////////////////////////
    //       VertexBufferLayout       //
    ////////////////////////////////////

    VertexBufferLayout::VertexBufferLayout(void)
      : m_stride(0)
    { }

    VertexBufferLayout::VertexBufferLayout(std::initializer_list<VertexAttribute> attributes)
    {
        std::ranges::copy(attributes, std::back_inserter(m_attributes));
        calculate_offsets_and_stride();
    }

    VertexBufferLayout::VertexBufferLayout(std::span<VertexAttribute> attributes)
    {
        std::ranges::copy(attributes, std::back_inserter(m_attributes));
        calculate_offsets_and_stride();
    }

    void VertexBufferLayout::add_attribute(const VertexAttribute& attribute)
    {
        m_attributes.push_back(attribute);
        calculate_offsets_and_stride();
    }

    uint VertexBufferLayout::num_attributes(void) const
    {
        return m_attributes.size();
    }

    void VertexBufferLayout::activate(void)
    {
        DRK_PROFILE_FUNCTION();

        for (uint index = 0; const auto& attribute : m_attributes)
        {
            const auto type  = attribute.type;
            const auto glType = to_gl_base_shader_data_type(type);
            const auto norm  = static_cast<GLboolean>(attribute.normalized);
            const void* ptr = DRK_BUFFER_OFFSET(attribute.offset);

            if (is_matrix(type))
            {
                const uint count = get_shader_data_type_attribute_count(type);
                for (int i = 0; i < count; ++i)
                {
                    glEnableVertexAttribArray(index);
                    glVertexAttribPointer(index, count, glType, norm, m_stride, ptr);
                    ++index;
                }
            }
            else
            {
                const uint count = get_shader_data_type_element_count(type);
                glEnableVertexAttribArray(index);

                if (to_shader_data_base_type(type) == ShaderDataBaseType::Float)
                    glVertexAttribPointer(index, count, glType, norm, m_stride, ptr);
                else
                    glVertexAttribIPointer(index, count, glType, m_stride, ptr);
                ++index;
            }
        }
    }

    void VertexBufferLayout::calculate_offsets_and_stride(void)
    {
        DRK_PROFILE_FUNCTION();

        uint offset = 0;
        m_stride = 0;

        for (auto& attribute : m_attributes)
        {
            attribute.offset = offset;

            offset += attribute.size;
            m_stride += attribute.size;
        }
    }

    //////////////////////////////
    //       VertexBuffer       //
    //////////////////////////////

    VertexBuffer::VertexBuffer(uint size, PrimitiveType type, VertexBufferLayout layout)
      : GlBuffer(size),
        m_primitiveType(type),
        m_layout(std::move(layout))
    {
        DRK_PROFILE_FUNCTION();

        bind();
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    VertexBuffer::VertexBuffer(uint size, PrimitiveType type, const VertexBufferLayout& layout, void* data, uint count)
      : GlBuffer(size, count),
        m_primitiveType(type),
        m_layout(layout)
    {
        DRK_PROFILE_FUNCTION();

        bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void VertexBuffer::set_layout(const VertexBufferLayout& layout)
    {
        DRK_ASSERT_DEBUG_NO_MSG(!has_layout());
        m_layout = layout;
    }

    bool VertexBuffer::has_layout(void) const
    {
        return m_layout.num_attributes() > 0;
    }

    void VertexBuffer::activate_layout(void)
    {
        DRK_ASSERT_DEBUG_NO_MSG(has_layout());

        bind();
        m_layout.activate();
    }

    void VertexBuffer::update(void* data, uint size)
    {
        DRK_PROFILE_FUNCTION();
        DRK_ASSERT_DEBUG_NO_MSG(has_layout());

        bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, std::min(size, get_size()), data);
    }

    PrimitiveType VertexBuffer::get_primitive_type(void) const
    {
        return m_primitiveType;
    }

    void VertexBuffer::bind(void) const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void VertexBuffer::unbind(void) const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    /////////////////////////////
    //       IndexBuffer       //
    /////////////////////////////

    IndexBuffer::IndexBuffer(uint size)
      : GlBuffer(size)
    {
        DRK_PROFILE_FUNCTION();

        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, get_size(), nullptr, GL_DYNAMIC_DRAW);
    }

    IndexBuffer::IndexBuffer(Index* indices, uint count)
      : GlBuffer(calculate_size(count), count)
    {
        DRK_PROFILE_FUNCTION();

        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, get_size(), indices, GL_STATIC_DRAW);
    }

    IndexBuffer::IndexBuffer(std::span<Index> indices)
      : IndexBuffer(indices.data(), indices.size())
    { }

    void IndexBuffer::update(Index* indices, uint count)
    {
        DRK_PROFILE_FUNCTION();

        bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, std::min(calculate_size(count), get_size()), indices);
    }

    void IndexBuffer::update(std::span<Index> indices)
    {
        update(indices.data(), indices.size());
    }

    void IndexBuffer::bind(void) const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void IndexBuffer::unbind(void) const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    uint IndexBuffer::calculate_size(uint count)
    {
        return count * sizeof(Index);
    }
}
