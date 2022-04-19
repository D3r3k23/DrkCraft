#include "Util.hpp"

#include <glad/glad.h>

namespace DrkCraft
{
    uint to_gl_primitive_type(PrimitiveType type)
    {
        using enum PrimitiveType;
        switch (type)
        {
            case Lines         : return GL_LINES;
            case LineStrip     : return GL_LINE_STRIP;
            case LineLoop      : return GL_LINE_LOOP;
            case Triangles     : return GL_TRIANGLES;
            case TriangleStrip : return GL_TRIANGLE_STRIP;
            case TriangleFan   : return GL_TRIANGLE_FAN;
            default:
                DRK_ASSERT_DEBUG(false, "Unknown PrimitiveType");
                return 0;
        }
    }

    ShaderDataBaseType to_shader_data_base_type(ShaderDataType type)
    {
        uint baseTypeCode = (static_cast<uint>(type) >> 8) & 0xF;
        DRK_ASSERT_DEBUG(0 <= baseTypeCode && baseTypeCode <= 3, "Could not find base type");

        return static_cast<ShaderDataBaseType>(baseTypeCode);
    }

    uint to_gl_base_shader_data_type(ShaderDataBaseType baseType)
    {
        switch (baseType)
        {
            case ShaderDataBaseType::Float : return GL_FLOAT;
            case ShaderDataBaseType::Int   : return GL_INT;
            case ShaderDataBaseType::Uint  : return GL_UNSIGNED_INT;
            case ShaderDataBaseType::Bool  : return GL_INT;
            default:
                DRK_ASSERT_DEBUG(false, "Unknown ShaderDataBaseType");
                return 0;
        }
    }

    uint to_gl_base_shader_data_type(ShaderDataType type)
    {
        ShaderDataBaseType baseType = to_shader_data_base_type(type);
        return to_gl_base_shader_data_type(baseType);
    }

    uint get_shader_data_type_element_count(ShaderDataType type)
    {
        return static_cast<uint>(type) & 0xFF;
    }

    uint get_shader_data_type_attribute_count(ShaderDataType type)
    {
        return (static_cast<uint>(type) >> 12) & 0xF;
    }

    uint get_shader_data_type_size(ShaderDataType type)
    {
        const uint count = get_shader_data_type_element_count(type);
        switch (to_shader_data_base_type(type))
        {
            case ShaderDataBaseType::Float : return count * sizeof(float);
            case ShaderDataBaseType::Int   : return count * sizeof(int32);
            case ShaderDataBaseType::Uint  : return count * sizeof(uint32);
            case ShaderDataBaseType::Bool  : return count * sizeof(int32);
            default:
                DRK_ASSERT_DEBUG(false, "Invalid ShaderDataBaseType");
                return 0;
        }
    }

    bool is_matrix(ShaderDataType type)
    {
        return get_shader_data_type_attribute_count(type) > 0;
    }
}
