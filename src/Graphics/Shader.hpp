#ifndef DRK_SHADER_HPP
#define DRK_SHADER_HPP

#include "Core/Base.hpp"

#include <glad/glad.h>

#include <filesystem>
#include <string>
#include <string_view>

namespace DrkCraft
{
    using ShaderID = GLuint;
    using ShaderProgramID = GLuint;

    enum class ShaderType
    {
        None,
        Vertex,
        Fragment,
        Geometry
    };

    ShaderType get_shader_type_from_string(std::string_view str);
    ShaderType find_shader_type_from_source(std::string_view source);

    GLenum get_gl_shader_type(ShaderType type);

    class Shader
    {
    public:
        Shader(std::filesystem::path path, ShaderType type=ShaderType::None);

        ShaderID get_id(void) const;
        ShaderType get_shader_type(void) const;

    private:
        bool compile(std::string_view source);

    private:
        ShaderID m_id;
        ShaderType m_type;
    };

    class ShaderProgram
    {
    public:
        ShaderProgram()=default;
        ShaderProgram(std::string_view name, const std::vector<Shader>& shaders);

        ShaderProgramID get_id(void) const;
        std::string get_name(void) const;

    private:
        bool link(void);

    private:
        ShaderProgramID m_id;
        std::string m_name;
    };
}

#endif // DRK_SHADER_HPP
