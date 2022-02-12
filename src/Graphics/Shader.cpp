#include "Shader.hpp"

#include "Util/File.hpp"
#include "Core/Debug/Profiler.hpp"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace DrkCraft
{
    uint to_gl_shader_type(ShaderType type)
    {
        switch (type)
        {
            case ShaderType::Vertex               : return GL_VERTEX_SHADER;
            case ShaderType::Fragment             : return GL_FRAGMENT_SHADER;
            case ShaderType::Geometry             : return GL_GEOMETRY_SHADER;
            case ShaderType::TesselationControl   : return GL_TESS_CONTROL_SHADER;
            case ShaderType::TesselationEvaluation: return GL_TESS_EVALUATION_SHADER;
            default:
                DRK_ASSERT_DEBUG(false, "Unknown shader type");
                return 0;
        }
    }

    std::string_view shader_type_to_string(ShaderType type)
    {
        switch (type)
        {
            case ShaderType::Vertex               : return "vertex shader";
            case ShaderType::Fragment             : return "fragment shader";
            case ShaderType::Geometry             : return "geometry shader";
            case ShaderType::TesselationControl   : return "tesselation control shader";
            case ShaderType::TesselationEvaluation: return "tesselation evaluation shader";
            default:
                DRK_ASSERT_DEBUG(false, "Unknown shader type");
                return "";
        }
    }

    ////////////////////////
    //       Shader       //
    ////////////////////////

    std::unordered_map<std::string, Ref<Shader>> Shader::s_shaderCache;

    Ref<Shader> Shader::create(const fs::path& path, ShaderType type)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Creating Shader from file {}", path.string());

        DRK_ASSERT_CORE(is_file(path), "Shader file does not exist");
        DRK_ASSERT_CORE(type != ShaderType::None, "Unknown shader type");

        if (s_shaderCache.contains(path.string()))
        {
            DRK_LOG_CORE_INFO("Shader found in cache");
            const auto& shader = s_shaderCache[path.string()];
            DRK_ASSERT_DEBUG(type == shader->get_type(), "Shader type does not match cached");
            return shader;
        }
        else
        {
            const std::string source = read_file(path);
            DRK_ASSERT_DEBUG(source.size() > 0, "Could not load shader file");

            auto shader = make_ref<Shader>(type);
            shader->compile(source);

            DRK_LOG_CORE_INFO("Adding Shader to cache");
            s_shaderCache[path.string()] = shader;
            return shader;
        }
    }

    Shader::Shader(ShaderType type)
    {
        GlObjectID id = glCreateShader(to_gl_shader_type(type));
        DRK_ASSERT_DEBUG(id, "glCreateShader failed");
        m_id = id;
        m_type = type;
    }

    Shader::~Shader(void)
    {
        glDeleteShader(m_id);
    }

    ShaderType Shader::get_type(void) const
    {
        return m_type;
    }

    void Shader::compile(std::string_view source)
    {
        DRK_PROFILE_FUNCTION();

        const auto shaderType = shader_type_to_string(m_type);
        DRK_LOG_CORE_TRACE("Compiling {}", shaderType);

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

            DRK_LOG_CORE_ERROR("{} compilation failed: {}", shaderType, shaderLog.data());
        }
    }

    ///////////////////////////////
    //       ShaderProgram       //
    ///////////////////////////////

    ShaderProgram::ShaderProgram(std::string_view name)
      : m_name(name)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Creating ShaderProgram {}", m_name);

        m_id = glCreateProgram();
        DRK_ASSERT_DEBUG(m_id, "glCreateProgram failed");
    }

    ShaderProgram::ShaderProgram(std::string_view name, std::initializer_list<Ref<Shader>> shaders)
      : ShaderProgram(name)
    {
        DRK_PROFILE_FUNCTION();
        std::ranges::copy(shaders, std::back_inserter(m_shaders));
    }

    ShaderProgram::ShaderProgram(std::string_view name, std::span<Ref<Shader>> shaders)
      : ShaderProgram(name)
    {
        DRK_PROFILE_FUNCTION();
        attach(shaders);
    }

    ShaderProgram::~ShaderProgram(void)
    {
        glDeleteProgram(m_id);
    }

    void ShaderProgram::attach(Ref<Shader> shader)
    {
        glAttachShader(m_id, shader->get_id());
        m_shaders.push_back(shader);
    }

    void ShaderProgram::attach(std::span<Ref<Shader>> shaders)
    {
        for (const auto& shader : shaders)
            attach(shader);
    }

    void ShaderProgram::link(void)
    {
        DRK_PROFILE_FUNCTION();
        DRK_LOG_CORE_TRACE("Linking ShaderProgram: {}", get_name());

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

    std::string_view ShaderProgram::get_name(void) const
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

    int ShaderProgram::get_uniform_location(const std::string& name)
    {
        if (m_uniformLocationCache.contains(name))
            return m_uniformLocationCache[name];
        else
        {
            int location = glGetUniformLocation(m_id, name.c_str());
            DRK_ASSERT_DEBUG(location != -1, "Uniform \"{}\" does not exist", name);
            m_uniformLocationCache[name] = location;
            return location;
        }
    }

    void ShaderProgram::upload_uniform(const std::string& name, float data)
    {
        int location = get_uniform_location(name);
        glUniform1f(location, data);
    }

    void ShaderProgram::upload_uniform(const std::string& name, const vec2& data)
    {
        int location = get_uniform_location(name);
        glUniform2f(location, data.x, data.y);
    }

    void ShaderProgram::upload_uniform(const std::string& name, const vec3& data)
    {
        int location = get_uniform_location(name);
        glUniform3f(location, data.x, data.y, data.z);
    }

    void ShaderProgram::upload_uniform(const std::string& name, const vec4& data)
    {
        int location = get_uniform_location(name);
        glUniform4f(location, data.x, data.y, data.z, data.w);
    }

    void ShaderProgram::upload_uniform(const std::string& name, const std::span<float> data)
    {
        int location = get_uniform_location(name);
        glUniform1fv(location, data.size(), data.data());
    }

    void ShaderProgram::upload_uniform(const std::string& name, int32 data)
    {
        int location = get_uniform_location(name);
        glUniform1i(location, data);
    }

    void ShaderProgram::upload_uniform(const std::string& name, const ivec2& data)
    {
        int location = get_uniform_location(name);
        glUniform2i(location, data.x, data.y);
    }

    void ShaderProgram::upload_uniform(const std::string& name, const ivec3& data)
    {
        int location = get_uniform_location(name);
        glUniform3i(location, data.x, data.y, data.z);
    }

    void ShaderProgram::upload_uniform(const std::string& name, const ivec4& data)
    {
        int location = get_uniform_location(name);
        glUniform4i(location, data.x, data.y, data.z, data.w);
    }

    void ShaderProgram::upload_uniform(const std::string& name, const std::span<int32> data)
    {
        int location = get_uniform_location(name);
        glUniform1iv(location, data.size(), data.data());
    }

    void ShaderProgram::upload_uniform(const std::string& name, uint32 data)
    {
        int location = get_uniform_location(name);
        glUniform1ui(location, data);
    }

    void ShaderProgram::upload_uniform(const std::string& name, const uvec2& data)
    {
        int location = get_uniform_location(name);
        glUniform2ui(location, data.x, data.y);
    }

    void ShaderProgram::upload_uniform(const std::string& name, const uvec3& data)
    {
        int location = get_uniform_location(name);
        glUniform3ui(location, data.x, data.y, data.z);
    }

    void ShaderProgram::upload_uniform(const std::string& name, const uvec4& data)
    {
        int location = get_uniform_location(name);
        glUniform4ui(location, data.x, data.y, data.z, data.w);
    }

    void ShaderProgram::upload_uniform(const std::string& name, const std::span<uint32> data)
    {
        int location = get_uniform_location(name);
        glUniform1uiv(location, data.size(), data.data());
    }

    void ShaderProgram::upload_uniform(const std::string& name, bool data)
    {
        upload_uniform(name, static_cast<int>(data));
    }

    void ShaderProgram::upload_uniform(const std::string& name, const bvec2& data)
    {
        upload_uniform(name, static_cast<ivec2>(data));
    }

    void ShaderProgram::upload_uniform(const std::string& name, const bvec3& data)
    {
        upload_uniform(name, static_cast<ivec3>(data));
    }

    void ShaderProgram::upload_uniform(const std::string& name, const bvec4& data)
    {
        upload_uniform(name, static_cast<ivec4>(data));
    }

    void ShaderProgram::upload_uniform(const std::string& name, const std::span<bool> data)
    {
        std::vector<int32> intData(data.begin(), data.end());
        upload_uniform(name, intData);
    }

    void ShaderProgram::upload_uniform(const std::string& name, const std::vector<bool> data)
    {
        std::vector<int32> intData(data.begin(), data.end());
        upload_uniform(name, intData);
    }

    void ShaderProgram::upload_uniform(const std::string& name, const mat2& data)
    {
        int location = get_uniform_location(name);
        glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(data));
    }

    void ShaderProgram::upload_uniform(const std::string& name, const mat3& data)
    {
        int location = get_uniform_location(name);
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(data));
    }

    void ShaderProgram::upload_uniform(const std::string& name, const mat4& data)
    {
        int location = get_uniform_location(name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
    }
}
