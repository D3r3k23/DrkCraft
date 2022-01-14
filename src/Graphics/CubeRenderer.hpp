#ifndef DRK_CUBE_RENDERER_HPP
#define DRK_CUBE_RENDERER_HPP

#include "Core/Base.hpp"
#include "GlBuffer.hpp"
#include "Transform.hpp"

namespace DrkCraft
{
    class CubeRenderer
    {
    public:
        CubeRenderer(void);
        void draw_cube(const Transform& transform);

    private:
        Ptr<VertexBuffer> m_cubeVertexBuffer;
        Ptr<IndexBuffer>  m_cubeIndexBuffer;
    };
}

#endif // DRK_CUBE_RENDERER_HPP
