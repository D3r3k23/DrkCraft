#include "Shader.hpp"

#include "Core/Util.hpp"
#include "Core/Profiler.hpp"

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
                DRK_LOG_CORE_WARN("Unknown shader type");
                return 0;
        }
    }

    Ref<Shader> Shader::create(std::filesystem::path path, ShaderType type)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_CORE_TRACE("Creating Shader from file {}", path.string());

        DRK_ASSERT_DEBUG(std::filesystem::is_regular_file(path), "Shader file does not exist");
        DRK_ASSERT_DEBUG(type != ShaderType::None, "Unknown shader type");

        const std::string source = read_file(path);
        DRK_ASSERT_DEBUG(source.size() > 0, "Could not load shader file");

        ShaderID id = glCreateShader(get_gl_shader_type(type));
        DRK_ASSERT_DEBUG(id, "glCreateShader failed");

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
        DRK_PROFILE_FUNCTION();

        const char* raw_source = source.data();
        const int length = source.size();
        glShaderSource(m_id, 1, &raw_source, &length);

        glCompileShader(m_id);
        GLint success = 0;
        glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            GLint shaderLogLength = 0;
            glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &shaderLogLength);

            std::vector<GLchar> shaderLog(shaderLogLength);
            glGetShaderInfoLog(m_id, shaderLogLength, nullptr, shaderLog.data());
            shaderLog[shaderLogLength - 1] = '\0';

            DRK_LOG_CORE_ERROR("Shader compilation failed: {}", shaderLog.data());
        }
    }

    ShaderProgram::ShaderProgram(std::string_view name)
      : m_name(name)
    {
        DRK_PROFILE_FUNCTION();

        DRK_LOG_CORE_TRACE("Creating ShaderProgram {}", m_name);

        m_id = glCreateProgram();
        DRK_ASSERT_DEBUG(m_id, "glCreateProgram failed");
    }

    ShaderProgram::ShaderProgram(std::string_view name, std::span<Ref<Shader>> shaders)
      : ShaderProgram(name)
    {
        DRK_PROFILE_FUNCTION();

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
        DRK_PROFILE_FUNCTION();

        glLinkProgram(m_id);
        GLint success = 0;
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            GLint programLogLength = 0;
            glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &programLogLength);

            std::vector<GLchar> programLog(programLogLength);
            glGetProgramInfoLog(m_id, programLogLength, nullptr, programLog.data());
            programLog[programLogLength - 1] = '\0';

            DRK_LOG_CORE_ERROR("Shader program \"{}\" linkage failed: {}", m_name, programLog.data());
        }
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

    void ShaderProgram::upload_uniform_int_array(const std::string& name, const std::span<int> data)
    {
        GLint location = get_uniform_location(name);
        glUniform1iv(location, data.size(), data.data());
    }

    void ShaderProgram::upload_uniform_int(const std::string& name, int data)
    {
        GLint location = get_uniform_location(name);
        glUniform1i(location, data);
    }

    void ShaderProgram::upload_uniform_ivec2(const std::string& name, const glm::ivec2& data)
    {
        GLint location = get_uniform_location(name);
        glUniform2i(location, data.x, data.y);
    }

    void ShaderProgram::upload_uniform_ivec3(const std::string& name, const glm::ivec3& data)
    {
        GLint location = get_uniform_location(name);
        glUniform3i(location, data.x, data.y, data.z);
    }

    void ShaderProgram::upload_uniform_ivec4(const std::string& name, const glm::ivec4& data)
    {
        GLint location = get_uniform_location(name);
        glUniform4i(location, data.x, data.y, data.z, data.w);
    }

    void ShaderProgram::upload_uniform_uint_array(const std::string& name, const std::span<uint> data)
    {
        GLint location = get_uniform_location(name);
        glUniform1uiv(location, data.size(), data.data());
    }

    void ShaderProgram::upload_uniform_uint(const std::string& name, uint data)
    {
        GLint location = get_uniform_location(name);
        glUniform1ui(location, data);
    }

    void ShaderProgram::upload_uniform_uvec2(const std::string& name, const glm::uvec2& data)
    {
        GLint location = get_uniform_location(name);
        glUniform2ui(location, data.x, data.y);
    }

    void ShaderProgram::upload_uniform_uvec3(const std::string& name, const glm::uvec3& data)
    {
        GLint location = get_uniform_location(name);
        glUniform3ui(location, data.x, data.y, data.z);
    }

    void ShaderProgram::upload_uniform_uvec4(const std::string& name, const glm::uvec4& data)
    {
        GLint location = get_uniform_location(name);
        glUniform4ui(location, data.x, data.y, data.z, data.w);
    }

    void ShaderProgram::upload_uniform_float_array(const std::string& name, const std::span<float> data)
    {
        GLint location = get_uniform_location(name);
        glUniform1fv(location, data.size(), data.data());
    }

    void ShaderProgram::upload_uniform_float(const std::string& name, float data)
    {
        GLint location = get_uniform_location(name);
        glUniform1f(location, data);
    }

    void ShaderProgram::upload_uniform_vec2(const std::string& name, const glm::vec2& data)
    {
        GLint location = get_uniform_location(name);
        glUniform2f(location, data.x, data.y);
    }

    void ShaderProgram::upload_uniform_vec3(const std::string& name, const glm::vec3& data)
    {
        GLint location = get_uniform_location(name);
        glUniform3f(location, data.x, data.y, data.z);
    }

    void ShaderProgram::upload_uniform_vec4(const std::string& name, const glm::vec4& data)
    {
        GLint location = get_uniform_location(name);
        glUniform4f(location, data.x, data.y, data.z, data.w);
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
            DRK_ASSERT_DEBUG(location != -1, "Uniform \"{}\" does not exist", name);
            m_uniformLocationCache[name] = location;
            return location;
        }
    }
}
