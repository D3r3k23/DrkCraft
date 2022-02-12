#ifndef DRK_GRAPHICS_DETAIL_UTIL_HPP
#define DRK_GRAPHICS_DETAIL_UTIL_HPP

#include "Core/Base.hpp"

namespace DrkCraft
{
    enum class PrimitiveType
    {
        None = 0,

        Lines,
        LineStrip,
        LineLoop,

        Triangles,
        TriangleStrip,
        TriangleFan
    };

    uint to_gl_primitive_type(PrimitiveType type);

    enum class ShaderDataBaseType
    {
        Float = 0,
        Int   = 1,
        Uint  = 2,
        Bool  = 3
    };

    // Bits [7:0]: num elements | [11:8]: base data type | [12]: matrix
    enum class ShaderDataType : uint16
    {
        None   = 0,
        Float  = 0x0'0'01, Int  = 0x0'1'01, Uint  = 0x0'2'01, Bool  = 0x0'3'01,
        Float2 = 0x0'0'02, Int2 = 0x0'1'02, Uint2 = 0x0'2'02, Bool2 = 0x0'3'02,
        Float3 = 0x0'0'03, Int3 = 0x0'1'03, Uint3 = 0x0'2'03, Bool3 = 0x0'3'03,
        Float4 = 0x0'0'04, Int4 = 0x0'1'04, Uint4 = 0x0'2'04, Bool4 = 0x0'3'04,
        Mat2   = 0x1'0'04,
        Mat3   = 0x1'0'09,
        Mat4   = 0x1'0'10
    };

    ShaderDataBaseType to_shader_data_base_type(ShaderDataType type);
    uint to_gl_base_shader_data_type(ShaderDataBaseType baseType);
    uint to_gl_base_shader_data_type(ShaderDataType type);

    uint get_shader_data_type_element_count(ShaderDataType type);
    uint get_shader_data_type_size(ShaderDataType type);
}

#endif // DRK_GRAPHICS_DETAIL_UTIL_HPP
