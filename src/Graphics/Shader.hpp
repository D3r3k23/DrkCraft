#ifndef DRK_SHADER_HPP
#define DRK_SHADER_HPP

#include "Core/Base.hpp"

#include <glad/glad.h>

#include <unordered_map>
#include <vector>
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
        Geometry,
        TesselationControl,
        TesselationEvaluation
    };

    GLenum get_gl_shader_type(ShaderType type);

    class Shader
    {
    public:
        Shader(std::filesystem::path path, ShaderType type);
        Shader(const Shader&) = default;
        Shader(void) = default; // Required for std::unordered_map::operator[]

        ShaderID get_id(void) const;
        ShaderType get_type(void) const;

    private:
        bool compile(std::string_view source);

    private:
        ShaderID m_id = 0;
        ShaderType m_type{ShaderType::None};

        static std::unordered_map<std::string, Shader> s_compiledShaders;
    };

    class ShaderProgram
    {
    public:
        ShaderProgram(std::string_view name, const std::vector<Shader>& shaders);
        ShaderProgram(const ShaderProgram&) = default;
        ShaderProgram(void) = default; // Required for std::unordered_map::operator[]

        ShaderProgramID get_id(void) const;
        std::string get_name(void) const;

    private:
        bool link(void);

    private:
        ShaderProgramID m_id = 0;
        std::string m_name;
    };
}

#endif // DRK_SHADER_HPP
