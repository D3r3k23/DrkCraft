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

    enum class ShaderDataType : uint16
    {
        None = 0,

        Bool  = 0b00'001, Int  = 0b01'001, Uint  = 0b10'001, Float  = 0b11'001,
        Bool2 = 0b00'010, Int2 = 0b01'010, Uint2 = 0b10'010, Float2 = 0b11'010,
        Bool3 = 0b00'011, Int3 = 0b01'011, Uint3 = 0b10'011, Float3 = 0b11'011,
        Bool4 = 0b00'100, Int4 = 0b01'100, Uint4 = 0b10'100, Float4 = 0b11'100,
        Mat3, Mat4
    };

    uint to_shader_data_type_size(ShaderDataType type);
    uint to_shader_data_type_count(ShaderDataType type);
    GLenum to_gl_base_shader_data_type(ShaderDataType type);

    class Shader : public GlObject
    {
    public:
        static Ref<Shader> create(const std::filesystem::path& path, ShaderType type);
        Shader(ShaderType type); // Required for make_ptr, should not use directly
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

        void bind(void);
        void unbind(void);

        GLint get_uniform_location(const std::string& name);

        void upload_uniform(const std::string& name, bool data);
        void upload_uniform(const std::string& name, const glm::bvec2& data);
        void upload_uniform(const std::string& name, const glm::bvec3& data);
        void upload_uniform(const std::string& name, const glm::bvec4& data);
        void upload_uniform(const std::string& name, const std::span<bool> data);
        void upload_uniform(const std::string& name, const std::vector<bool> data);

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

        void upload_uniform(const std::string& name, float data);
        void upload_uniform(const std::string& name, const glm::vec2& data);
        void upload_uniform(const std::string& name, const glm::vec3& data);
        void upload_uniform(const std::string& name, const glm::vec4& data);
        void upload_uniform(const std::string& name, const std::span<float> data);

        void upload_uniform(const std::string& name, const glm::mat2& data);
        void upload_uniform(const std::string& name, const glm::mat3& data);
        void upload_uniform(const std::string& name, const glm::mat4& data);

    private:
        std::string m_name;

        std::vector<Ref<Shader>> m_shaders;

        std::unordered_map<std::string, GLint> m_uniformLocationCache;
    };
}

#endif // DRK_SHADER_HPP
