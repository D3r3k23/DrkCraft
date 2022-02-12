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
        uint baseTypeCode = (static_cast<uint>(type) >> 8) & 0xFF;
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
        return static_cast<uint>(type) & 0xF;
    }

    uint get_shader_data_type_size(ShaderDataType type)
    {
        uint count = get_shader_data_type_element_count(type);
        switch (type)
        {
            case ShaderDataType::Float  :
            case ShaderDataType::Float2 :
            case ShaderDataType::Float3 :
            case ShaderDataType::Float4 : return count * sizeof(float);
            case ShaderDataType::Int    :
            case ShaderDataType::Int2   :
            case ShaderDataType::Int3   :
            case ShaderDataType::Int4   : return count * sizeof(int32);
            case ShaderDataType::Uint   :
            case ShaderDataType::Uint2  :
            case ShaderDataType::Uint3  :
            case ShaderDataType::Uint4  : return count * sizeof(uint32);
            case ShaderDataType::Bool   :
            case ShaderDataType::Bool2  :
            case ShaderDataType::Bool3  :
            case ShaderDataType::Bool4  :
            case ShaderDataType::Mat2   :
            case ShaderDataType::Mat3   :
            case ShaderDataType::Mat4   : return count * sizeof(float);
            default:
                DRK_ASSERT_DEBUG(false, "Invalid ShaderDataType");
                return 0;
        }
    }
}
