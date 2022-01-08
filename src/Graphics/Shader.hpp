#ifndef DRK_SHADER_HPP
#define DRK_SHADER_HPP

#include "Core/Base.hpp"

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
    using ShaderID = GLuint;
    using ShaderProgramID = GLuint;

    enum class ShaderType
    {
        None,
        Vertex,
        Fragment,
        Geometry,
        TesselationControl,
        TesselationEvaluation
    };

    GLenum get_gl_shader_type(ShaderType type);

    class Shader
    {
    public:
        static Ref<Shader> create(std::filesystem::path path, ShaderType type);
        ~Shader(void);
        Shader(void) = default; // Required for make_ptr, should not use directly

        Shader(const Shader&) = delete;
        Shader(Shader&&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader& operator=(Shader&&) = delete;

        ShaderID get_id(void) const;
        ShaderType get_type(void) const;

    private:
        void compile(std::string_view source);

    private:
        ShaderID m_id = 0;
        ShaderType m_type = ShaderType::None;
    };

    class ShaderProgram
    {
    public:
        ShaderProgram(std::string_view name);
        ShaderProgram(std::string_view name, std::span<Ref<Shader>> shaders);
        ~ShaderProgram(void);

        ShaderProgram(void) = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram(ShaderProgram&&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        ShaderProgram& operator=(ShaderProgram&&) = delete;

        void add_shader(Ref<Shader> shader);
        void link(void) const;

        std::string get_name(void) const;

        void bind(void) const;
        void unbind(void) const;

        void upload_uniform_int_array(const std::string& name, const std::span<int> data);
        void upload_uniform_int(const std::string& name, int data);
        void upload_uniform_ivec2(const std::string& name, const glm::ivec2& data);
        void upload_uniform_ivec3(const std::string& name, const glm::ivec3& data);
        void upload_uniform_ivec4(const std::string& name, const glm::ivec4& data);

        void upload_uniform_uint_array(const std::string& name, const std::span<uint> data);
        void upload_uniform_uint(const std::string& name, uint data);
        void upload_uniform_uvec2(const std::string& name, const glm::uvec2& data);
        void upload_uniform_uvec3(const std::string& name, const glm::uvec3& data);
        void upload_uniform_uvec4(const std::string& name, const glm::uvec4& data);

        void upload_uniform_float_array(const std::string& name, const std::span<float> data);
        void upload_uniform_float(const std::string& name, float data);
        void upload_uniform_vec2(const std::string& name, const glm::vec2& data);
        void upload_uniform_vec3(const std::string& name, const glm::vec3& data);
        void upload_uniform_vec4(const std::string& name, const glm::vec4& data);

        void upload_uniform_mat2(const std::string& name, const glm::mat2& data);
        void upload_uniform_mat3(const std::string& name, const glm::mat3& data);
        void upload_uniform_mat4(const std::string& name, const glm::mat4& data);

    private:
        GLint get_uniform_location(const std::string& name);

    private:
        ShaderProgramID m_id = 0;
        std::string m_name;

        std::vector<Ref<Shader>> m_shaders;

        std::unordered_map<std::string, GLint> m_uniformLocationCache;
    };
}

#endif // DRK_SHADER_HPP
