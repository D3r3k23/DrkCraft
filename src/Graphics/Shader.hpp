#ifndef DRK_GRAPHICS_SHADER_HPP
#define DRK_GRAPHICS_SHADER_HPP

#include "Core/Base.hpp"
#include "Graphics/detail/GlObject.hpp"
#include "Graphics/detail/Util.hpp"

#include "lib/fs.hpp"

#include "lib/glm/vec.hpp"
#include "lib/glm/mat.hpp"

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <span>

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

    uint to_gl_shader_type(ShaderType type);
    std::string_view shader_type_to_string(ShaderType type);

    class Shader : public GlObject
    {
    public:
        static Ref<Shader> create(const fs::path& path, ShaderType type);

    private:
        Shader(ShaderType type);

    public:
        virtual ~Shader(void);

        ShaderType get_type(void) const;
        Result compile(std::string_view source);

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

        void attach(Ref<Shader> shader);
        void attach(std::span<Ref<Shader>> shaders);
        Result link(void);

        std::string_view get_name(void) const;

        virtual void bind(void) const override;
        virtual void unbind(void) const override;

    private:
        int get_uniform_location(const std::string& name);

    private:
        std::string m_name;

        std::vector<Ref<Shader>> m_shaders;

        std::unordered_map<std::string, int> m_uniformLocationCache;

    public:
        void upload_uniform(const std::string& name, float data);
        void upload_uniform(const std::string& name, const vec2& data);
        void upload_uniform(const std::string& name, const vec3& data);
        void upload_uniform(const std::string& name, const vec4& data);
        void upload_uniform(const std::string& name, const std::span<float> data);

        void upload_uniform(const std::string& name, int32 data);
        void upload_uniform(const std::string& name, const ivec2& data);
        void upload_uniform(const std::string& name, const ivec3& data);
        void upload_uniform(const std::string& name, const ivec4& data);
        void upload_uniform(const std::string& name, const std::span<int32> data);

        void upload_uniform(const std::string& name, uint32 data);
        void upload_uniform(const std::string& name, const uvec2& data);
        void upload_uniform(const std::string& name, const uvec3& data);
        void upload_uniform(const std::string& name, const uvec4& data);
        void upload_uniform(const std::string& name, const std::span<uint32> data);

        void upload_uniform(const std::string& name, bool data);
        void upload_uniform(const std::string& name, const bvec2& data);
        void upload_uniform(const std::string& name, const bvec3& data);
        void upload_uniform(const std::string& name, const bvec4& data);
        void upload_uniform(const std::string& name, const std::span<bool> data);
        void upload_uniform(const std::string& name, const std::vector<bool> data);

        void upload_uniform(const std::string& name, const mat2& data);
        void upload_uniform(const std::string& name, const mat3& data);
        void upload_uniform(const std::string& name, const mat4& data);
    };
}

#endif // DRK_GRAPHICS_SHADER_HPP
