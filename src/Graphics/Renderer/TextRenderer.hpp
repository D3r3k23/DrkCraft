#ifndef DRK_GRAPHICS_RENDERER_TEXT_RENDERER
#define DRK_GRAPHICS_RENDERER_TEXT_RENDERER

#include "Core/Base.hpp"

namespace DrkCraft
{
    class TextRenderer
    {
    public:
        static void init(void);
        static void shutdown(void);

        static void begin_scene(void);
        static void end_scene(void);
}

#endif // DRK_GRAPHICS_RENDERER_TEXT_RENDERER
