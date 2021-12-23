#ifndef DRK_CUBE_RENDERER_HPP
#define DRK_CUBE_RENDERER_HPP

#include "Core/Base.hpp"
#include "Buffer.hpp"
#include "Transform.hpp"

namespace DrkCraft
{
    class CubeRenderer
    {
    public:
        CubeRenderer(void);
        void draw_cube(const Transform& transform);

    private:
        Ptr<VertexBuffer> cubeVertexBuffer;
        Ptr<IndexBuffer>  cubeIndexBuffer;
    };
}

#endif // DRK_CUBE_RENDERER_HPP
