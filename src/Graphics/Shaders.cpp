#include "Shaders.hpp"

#include "Core/Util.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace DrkCraft
{
    GLenum get_gl_shader_type(ShaderType type)
    {
        switch (type)
        {
            case ShaderType::Vertex               : return GL_VERTEX_SHADER;
            case ShaderType::Fragment             : return GL_FRAGMENT_SHADER;
            case ShaderType::Geometry             : return GL_GEOMETRY_SHADER;
            case ShaderType::TesselationControl   : return GL_TESS_CONTROL_SHADER;
            case ShaderType::TesselationEvaluation: return GL_TESS_EVALUATION_SHADER;
            default:
                DRK_LOG_WARN("Unknown shader type");
                return 0;
        }
    }

    Ref<Shader> Shader::create(std::filesystem::path path, ShaderType type)
    {
        DRK_LOG_TRACE("Creating Shader from file {}", path.string());

        DRK_ASSERT(std::filesystem::is_regular_file(path), "Shader file does not exist");
        DRK_ASSERT(type != ShaderType::None, "Unknown shader type");

        const std::string source = read_file(path);
        DRK_ASSERT(source.size() > 0, "Could not load shader file");

        ShaderID id = glCreateShader(get_gl_shader_type(type));
        DRK_ASSERT(id, "glCreateShader failed");

        auto shader = make_ref<Shader>();
        shader->m_id = id;
        shader->m_type = type;
        shader->compile(source);
        return shader;
    }

    Shader::~Shader(void)
    {
        glDeleteShader(m_id);
    }

    ShaderID Shader::get_id(void) const
    {
        return m_id;
    }

    ShaderType Shader::get_type(void) const
    {
        return m_type;
    }

    void Shader::compile(std::string_view source)
    {
        const char* raw_source = source.data();
        const int length = source.size();
        glShaderSource(m_id, 1, &raw_source, &length);

        glCompileShader(m_id);
        GLint success;
        glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
        DRK_ASSERT(success, "Shader program linking failed");
    }

    ShaderProgram::ShaderProgram(std::string_view name)
      : m_name(name)
    {
        DRK_LOG_TRACE("Creating ShaderProgram {}", m_name);

        m_id = glCreateProgram();
        DRK_ASSERT(m_id, "glCreateProgram failed");
    }

    ShaderProgram::ShaderProgram(std::string_view name, std::span<Ref<Shader>> shaders)
      : ShaderProgram(name)
    {
        for (const auto& shader : shaders)
            add_shader(shader);
        link();
    }

    ShaderProgram::~ShaderProgram(void)
    {
        glDeleteProgram(m_id);
    }

    void ShaderProgram::add_shader(Ref<Shader> shader)
    {
        glAttachShader(m_id, shader->get_id());
        m_shaders.push_back(shader);
    }

    void ShaderProgram::link(void) const
    {
        glLinkProgram(m_id);
        GLint success;
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        DRK_ASSERT(success, "Shader program linkage failed");
    }

    std::string ShaderProgram::get_name(void) const
    {
        return m_name;
    }

    void ShaderProgram::bind(void) const
    {
        glUseProgram(m_id);
    }

    void ShaderProgram::unbind(void) const
    {
        glUseProgram(0);
    }

    void ShaderProgram::upload_uniform_int(const std::string& name, int data)
    {
        GLint location = get_uniform_location(name);
        glUniform1i(location, data);
    }

    void ShaderProgram::upload_uniform_int2(const std::string& name, const glm::ivec2& data)
    {
        GLint location = get_uniform_location(name);
        glUniform2i(location, data.x, data.y);
    }

    void ShaderProgram::upload_uniform_int3(const std::string& name, const glm::ivec3& data)
    {
        GLint location = get_uniform_location(name);
        glUniform3i(location, data.x, data.y, data.z);
    }

    void ShaderProgram::upload_uniform_int4(const std::string& name, const glm::ivec4& data)
    {
        GLint location = get_uniform_location(name);
        glUniform4i(location, data.x, data.y, data.z, data.w);
    }

    void ShaderProgram::upload_uniform_int_array(const std::string& name, const std::span<int> data)
    {
        GLint location = get_uniform_location(name);
        glUniform1iv(location, data.size(), data.data());
    }

    void ShaderProgram::upload_uniform_uint(const std::string& name, uint data)
    {
        GLint location = get_uniform_location(name);
        glUniform1ui(location, data);
    }

    void ShaderProgram::upload_uniform_uint2(const std::string& name, const glm::uvec2& data)
    {
        GLint location = get_uniform_location(name);
        glUniform2ui(location, data.x, data.y);
    }

    void ShaderProgram::upload_uniform_uint3(const std::string& name, const glm::uvec3& data)
    {
        GLint location = get_uniform_location(name);
        glUniform3ui(location, data.x, data.y, data.z);
    }

    void ShaderProgram::upload_uniform_uint4(const std::string& name, const glm::uvec4& data)
    {
        GLint location = get_uniform_location(name);
        glUniform4ui(location, data.x, data.y, data.z, data.w);
    }

    void ShaderProgram::upload_uniform_uint_array(const std::string& name, const std::span<uint> data)
    {
        GLint location = get_uniform_location(name);
        glUniform1uiv(location, data.size(), data.data());
    }

    void ShaderProgram::upload_uniform_float(const std::string& name, float data)
    {
        GLint location = get_uniform_location(name);
        glUniform1f(location, data);
    }

    void ShaderProgram::upload_uniform_float2(const std::string& name, const glm::vec2& data)
    {
        GLint location = get_uniform_location(name);
        glUniform2f(location, data.x, data.y);
    }

    void ShaderProgram::upload_uniform_float3(const std::string& name, const glm::vec3& data)
    {
        GLint location = get_uniform_location(name);
        glUniform3f(location, data.x, data.y, data.z);
    }

    void ShaderProgram::upload_uniform_float4(const std::string& name, const glm::vec4& data)
    {
        GLint location = get_uniform_location(name);
        glUniform4f(location, data.x, data.y, data.z, data.w);
    }

    void ShaderProgram::upload_uniform_float_array(const std::string& name, const std::span<float> data)
    {
        GLint location = get_uniform_location(name);
        glUniform1fv(location, data.size(), data.data());
    }

    void ShaderProgram::upload_uniform_mat2(const std::string& name, const glm::mat2& data)
    {
        GLint location = get_uniform_location(name);
        glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(data));
    }

    void ShaderProgram::upload_uniform_mat3(const std::string& name, const glm::mat3& data)
    {
        GLint location = get_uniform_location(name);
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(data));
    }

    void ShaderProgram::upload_uniform_mat4(const std::string& name, const glm::mat4& data)
    {
        GLint location = get_uniform_location(name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
    }

    GLint ShaderProgram::get_uniform_location(const std::string& name)
    {
        if (m_uniformLocationCache.contains(name))
            return m_uniformLocationCache[name];
        else
        {
            GLint location = glGetUniformLocation(m_id, name.c_str());
            DRK_ASSERT(location != -1, "Uniform does not exist");
            m_uniformLocationCache[name] = location;
            return location;
        }
    }
}
