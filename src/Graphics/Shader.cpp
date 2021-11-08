#include "Shader.hpp"

#include "Core/Util.hpp"

#include <sstream>
#include <algorithm>

namespace DrkCraft
{
    ShaderType get_shader_type_from_string(std::string_view str)
    {
        if      (str == "vertex")   return ShaderType::Vertex;
        else if (str == "fragment") return ShaderType::Fragment;
        else if (str == "geometry") return ShaderType::Geometry;

        DRK_LOG_WARN("Unknown shader type \"{}\"", str);
        return ShaderType::None;
    }

    ShaderType find_shader_type_from_source(std::string_view source)
    {
        std::stringstream ss(source.data());
        const std::string token = "#type";
        std::string line;
        while (std::getline(ss, line))
        {
            size_t tokenPos;
            if ((tokenPos = line.find(token)) != std::string::npos)
            {
                size_t EOL = line.find_first_of("\r\n", tokenPos);
                DRK_ASSERT(EOL != std::string::npos, "\"#type\" syntax error in shader");

                size_t typePos = tokenPos + token.size() + 1;
                std::string typeStr = line.substr(typePos, EOL - typePos);
                return get_shader_type_from_string(typeStr);
            }
        }
        return ShaderType::None;
    }

    GLenum get_gl_shader_type(ShaderType type)
    {
        switch (type)
        {
            case ShaderType::Vertex   : return GL_VERTEX_SHADER;
            case ShaderType::Fragment : return GL_FRAGMENT_SHADER;
            case ShaderType::Geometry : return GL_GEOMETRY_SHADER;
            default:
                DRK_LOG_WARN("Unknown shader type");
                return 0;
        }
    }

    std::unordered_map<std::string, Shader> Shader::s_compiledShaders;

    Shader::Shader(std::filesystem::path path, ShaderType type)
    {
        DRK_LOG_TRACE("Creating Shader from {}", path.string());

        if (s_compiledShaders.contains(path.string()))
        {
            auto compiledShader = s_compiledShaders[path.string()];
            DRK_LOG_INFO("Shader \"{}\" was already compiled", path.string());
            DRK_ASSERT(compiledShader.get_type() == type, "Compiled shader type does not match");

            m_id   = compiledShader.get_id();
            m_type = compiledShader.get_type();
            DRK_LOG_INFO("Copied already-compiled shader {}", path.string());
        }
        else
        {
            const std::string source = read_file(path);
            DRK_ASSERT(source.size() > 0, "Could not load shader file");

            if (type == ShaderType::None)
            {
                m_type = find_shader_type_from_source(source);
                DRK_ASSERT(m_type != ShaderType::None, "Could not determine type of shader");
            }
            else
                m_type = type;

            m_id = glCreateShader(get_gl_shader_type(m_type));
            DRK_ASSERT(m_id, "glCreateShader failed");

            if (compile(source))
            {
                DRK_LOG_INFO("Shader {} compiled successfully", path.string());
                s_compiledShaders[path.string()] = *this;
            }
            else
                DRK_ASSERT_FALSE("Shader compilation failed");
        }
    }

    ShaderID Shader::get_id(void) const
    {
        return m_id;
    }

    ShaderType Shader::get_type(void) const
    {
        return m_type;
    }

    bool Shader::compile(std::string_view source)
    {
        const char* raw_source = source.data();
        const int length = source.size();
        glShaderSource(m_id, 1, &raw_source, &length);

        glCompileShader(m_id);
        GLint success;
        glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
        return success;
    }

    ShaderProgram::ShaderProgram(std::string_view name, const std::vector<Shader>& shaders)
      : m_name(name)
    {
        DRK_LOG_TRACE("Creating ShaderProgram {}", m_name);

        m_id = glCreateProgram();
        DRK_ASSERT(m_id, "glCreateProgram failed");

        for (const auto shader : shaders)
            glAttachShader(m_id, shader.get_id());

        if (link())
            DRK_LOG_INFO("Shader program {} linked successfully", m_name);
        else
            DRK_ASSERT_FALSE("Shader program linking failed");
    }

    ShaderProgramID ShaderProgram::get_id(void) const
    {
        return m_id;
    }

    std::string ShaderProgram::get_name(void) const
    {
        return m_name;
    }

    bool ShaderProgram::link(void)
    {
        glLinkProgram(m_id);
        GLint success;
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        return success;
    }
}
