#ifndef DRK_SHADER_HPP
#define DRK_SHADER_HPP

#include "Core/Base.hpp"
#include "GlObject.hpp"

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <span>
#include <filesystem>

namespace DrkCraft
{
    enum class ShaderType
    {
        None,
        Vertex,
        Fragment,
        Geometry,
        TesselationControl,
        TesselationEvaluation
    };

    GLenum to_gl_shader_type(ShaderType type);
    std::string_view shader_type_to_string(ShaderType type);

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
    GLenum to_gl_base_shader_data_type(ShaderDataBaseType baseType);
    GLenum to_gl_base_shader_data_type(ShaderDataType type);

    uint get_shader_data_type_element_count(ShaderDataType type);
    uint get_shader_data_type_size(ShaderDataType type);

    class Shader : public GlObject
    {
    public:
        static Ref<Shader> create(const std::filesystem::path& path, ShaderType type);
        Shader(ShaderType type); // Required for make_ref, should not use directly
        virtual ~Shader(void);

        Shader(const Shader&) = delete;
        Shader(Shader&&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader& operator=(Shader&&) = delete;

        ShaderType get_type(void) const;

    private:
        void compile(std::string_view source);

    private:
        ShaderType m_type = ShaderType::None;

        static std::unordered_map<std::string, Ref<Shader>> s_shaderCache;
    };

    class ShaderProgram : public GlObject
    {
    public:
        ShaderProgram(std::string_view name);
        ShaderProgram(std::string_view name, std::span<Ref<Shader>> shaders);
        virtual ~ShaderProgram(void);

        ShaderProgram(void) = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram(ShaderProgram&&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        ShaderProgram& operator=(ShaderProgram&&) = delete;

        void attach(Ref<Shader> shader);
        void attach(std::span<Ref<Shader>> shaders);
        void link(void);

        std::string_view get_name(void) const;

        virtual void bind(void) const override;
        virtual void unbind(void) const override;

    private:
        GLint get_uniform_location(const std::string& name);

    private:
        std::string m_name;

        std::vector<Ref<Shader>> m_shaders;

        std::unordered_map<std::string, GLint> m_uniformLocationCache;

    public:
        void upload_uniform(const std::string& name, float data);
        void upload_uniform(const std::string& name, const glm::vec2& data);
        void upload_uniform(const std::string& name, const glm::vec3& data);
        void upload_uniform(const std::string& name, const glm::vec4& data);
        void upload_uniform(const std::string& name, const std::span<float> data);

        void upload_uniform(const std::string& name, int32 data);
        void upload_uniform(const std::string& name, const glm::ivec2& data);
        void upload_uniform(const std::string& name, const glm::ivec3& data);
        void upload_uniform(const std::string& name, const glm::ivec4& data);
        void upload_uniform(const std::string& name, const std::span<int32> data);

        void upload_uniform(const std::string& name, uint32 data);
        void upload_uniform(const std::string& name, const glm::uvec2& data);
        void upload_uniform(const std::string& name, const glm::uvec3& data);
        void upload_uniform(const std::string& name, const glm::uvec4& data);
        void upload_uniform(const std::string& name, const std::span<uint32> data);

        void upload_uniform(const std::string& name, bool data);
        void upload_uniform(const std::string& name, const glm::bvec2& data);
        void upload_uniform(const std::string& name, const glm::bvec3& data);
        void upload_uniform(const std::string& name, const glm::bvec4& data);
        void upload_uniform(const std::string& name, const std::span<bool> data);
        void upload_uniform(const std::string& name, const std::vector<bool> data);

        void upload_uniform(const std::string& name, const glm::mat2& data);
        void upload_uniform(const std::string& name, const glm::mat3& data);
        void upload_uniform(const std::string& name, const glm::mat4& data);
    };
}

#endif // DRK_SHADER_HPP
