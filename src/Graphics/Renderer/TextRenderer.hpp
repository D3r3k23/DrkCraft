#ifndef DRK_GRAPHICS_RENDERER_TEXT_RENDERER_HPP
#define DRK_GRAPHICS_RENDERER_TEXT_RENDERER_HPP

#include "Core/Base.hpp"
#include "Graphics/Renderer/Renderer.hpp"

namespace DrkCraft
{
    class TextRenderer : public Renderer
    {
    public:
        static void init(void);
        static void shutdown(void);

        static void begin_frame(void);
        static void end_frame(void);

        static void begin_scene(void);
        static void end_scene(void);
    };
}

#endif // DRK_GRAPHICS_RENDERER_TEXT_RENDERER_HPP
