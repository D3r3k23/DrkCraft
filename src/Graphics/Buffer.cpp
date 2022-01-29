#include "Buffer.hpp"

#include "Core/Profiler.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

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

    VertexAttribute::VertexAttribute(std::string_view name, ShaderDataType type, bool normalized)
      : name(name),
        type(type),
        size(get_shader_data_type_size(type)),
        count(get_shader_data_type_element_count(type)),
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
        calc_offsets_and_stride();
    }

    VertexBufferLayout::VertexBufferLayout(std::span<VertexAttribute> attributes)
    {
        add_attribute(attributes);
    }

    void VertexBufferLayout::add_attribute(const VertexAttribute& attribute)
    {
        m_attributes.push_back(attribute);
        calc_offsets_and_stride();
    }

    void VertexBufferLayout::add_attribute(std::span<VertexAttribute> attributes)
    {
        std::ranges::copy(attributes, std::back_inserter(m_attributes));
        calc_offsets_and_stride();
    }

    void VertexBufferLayout::activate(void)
    {
        for (uint i = 0; const auto& attribute : m_attributes)
        {
            ShaderDataBaseType baseType = to_shader_data_base_type(attribute.type);

            switch (baseType)
            {
                case ShaderDataBaseType::Float:
                    glVertexAttribPointer(i,
                        attribute.count,
                        to_gl_base_shader_data_type(baseType),
                        static_cast<GLboolean>(attribute.normalized),
                        m_stride,
                        reinterpret_cast<const void*>(attribute.offset)
                    );
                    break;
                case ShaderDataBaseType::Int:
                case ShaderDataBaseType::Uint:
                case ShaderDataBaseType::Bool:
                    glVertexAttribIPointer(i,
                        attribute.count,
                        to_gl_base_shader_data_type(baseType),
                        m_stride,
                        reinterpret_cast<const void*>(attribute.offset)
                    );
                    break;
                // Mat?
                default:
                    DRK_ASSERT_DEBUG(false, "Invalid base type");
                    continue;
            }
            i++;
        }
    }

    void VertexBufferLayout::calc_offsets_and_stride(void)
    {
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

    VertexBuffer::VertexBuffer(uint size)
      : GlBuffer(size)
    {
        DRK_PROFILE_FUNCTION();

        bind();
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        unbind();
    }

    VertexBuffer::VertexBuffer(const VertexBufferLayout& layout, uint size)
      : GlBuffer(size),
        m_layout(layout)
    {
        DRK_PROFILE_FUNCTION();

        bind();
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        unbind();
    }

    VertexBuffer::VertexBuffer(const VertexBufferLayout& layout, void* data, uint count, uint size)
      : GlBuffer(size, count),
        m_layout(layout)
    {
        DRK_PROFILE_FUNCTION();

        bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
        unbind();
    }

    void VertexBuffer::set_layout(const VertexBufferLayout& layout)
    {
        DRK_ASSERT_DEBUG_NO_MSG(!has_layout());
        m_layout = layout;
    }

    void VertexBuffer::activate_layout(void)
    {
        DRK_ASSERT_DEBUG_NO_MSG(has_layout());

        glEnableVertexAttribArray(m_id);
        m_layout.activate();
    }

    void VertexBuffer::set_data(void* data, uint size)
    {
        DRK_ASSERT_DEBUG_NO_MSG(has_layout());
        if (size > get_size())
            size = get_size();
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
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

    IndexBuffer::IndexBuffer(std::span<Index> indices)
      : IndexBuffer(indices.data(), indices.size())
    { }

    IndexBuffer::IndexBuffer(Index* indices, uint count)
      : GlBuffer(count * sizeof(Index), count)
    {
        DRK_PROFILE_FUNCTION();

        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, get_size(), indices, GL_STATIC_DRAW);
        unbind();
    }

    void IndexBuffer::bind(void) const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void IndexBuffer::unbind(void) const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
