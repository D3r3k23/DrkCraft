#ifndef DRK_TRANSFORM_HPP
#define DRK_TRANSFORM_HPP

#include <glm/mat4x4.hpp>

namespace DrkCraft
{
    class Transform
    {
    private:
        glm::mat4 matrix;

    public:
        Transform(glm::mat4 matrix) : matrix(matrix) { }

        static Transform Translation(double x, double y, double z);
        static Transform Scale(double s);
        static Transform Rotation(double theta);
        static Transform Normal(void);
    };
}

#endif // DRK_TRANSFORM_HPP
