#ifndef DRK_RENDER_COMMAND_HPP
#define DRK_RENDER_COMMAND_HPP

#include "Core/Base.hpp"
#include "VertexArray.hpp"

#include <glm/vec3.hpp>

#include <functional>
#include <concepts>

namespace DrkCraft
{
    enum class RenderCommandType
    {
        None = 0,
        VertexArray,
        Cube
    };

    struct RenderCommand
    {
        static RenderCommandType static_type(void) { return RenderCommandType::None; }
        virtual RenderCommandType get_type(void) const = 0;
    };

    struct VertexArrayRenderCommand final : RenderCommand
    {
        static RenderCommandType static_type(void)
            { return RenderCommandType::VertexArray; }

        virtual RenderCommandType get_type(void) const override
            { return RenderCommandType::VertexArray; }

        const Ref<VertexArray>& vao;
    };

    struct CubeRenderCommand final : RenderCommand
    {
        static RenderCommandType static_type(void)
            { return RenderCommandType::Cube; }

        virtual RenderCommandType get_type(void) const override
            { return RenderCommandType::Cube; }

        const glm::vec3 position;
    };

    template <typename R>
    concept RenderCommandConcept = std::derived_from<R, RenderCommand> && std::is_final<R>::value;

    template <RenderCommandConcept R>
    using RenderCommandFn = std::function<void(const R&)>;

    class RenderCommandDispatcher
    {
    public:
        RenderCommandDispatcher(const RenderCommand& command)
          : command(command)
        { }

        template <RenderCommandConcept R>
        void dispatch(const RenderCommandFn<R>& fn)
        {
            if (R::static_type() == command.get_type();
                fn(command);
        }

    private:
        const RenderCommand& command;
    };
}

#endif // DRK_RENDER_COMMAND_HPP
