#ifndef DRK_GRAPHICS_TRANSFORM_HPP
#define DRK_GRAPHICS_TRANSFORM_HPP

#include "lib/glm/mat4.hpp"
#include "lib/glm/vec3.hpp"

namespace DrkCraft
{
    class Transform
    {
    private:
        mat4 m_matrix;

    public:
        Transform(void); // Identity matrix
        Transform(const mat4& matrix);

        Transform(const Transform&) = default;
        Transform(Transform&&) = default;
        Transform& operator=(const Transform&) = default;
        Transform& operator=(Transform&&) = default;

        static Transform Identity(void);
        static Transform Translation(float x, float y, float z);
        static Transform Translation(const vec3& d);
        static Transform Rotation(float theta, const vec3& axis);
        static Transform Scale(float s);
        static Transform Normal(void);

        Transform& translate(float x, float y, float z);
        Transform& translate(const vec3& d);
        Transform& rotate(float theta, const vec3& axis);
        Transform& scale(float s);

        operator mat4(void) const;

        Transform operator*(const Transform& other) const;
        Transform& operator*=(const Transform& other);
    };
}

#endif // DRK_GRAPHICS_TRANSFORM_HPP
