#ifndef DRK_TRANSFORM_HPP
#define DRK_TRANSFORM_HPP

#include <glm/mat4x4.hpp>

namespace DrkCraft
{
    class Transform
    {
    private:
        glm::mat4 m_matrix;

    public:
        Transform(void); // Identity matrix
        Transform(const glm::mat4& matrix);

        Transform(const Transform&) = default;
        Transform(Transform&&) = default;
        Transform& operator=(const Transform&) = default;
        Transform& operator=(Transform&&) = default;

        static Transform Identity(void);
        static Transform Translation(float x, float y, float z);
        static Transform Rotation(float theta, const glm::vec3& axis);
        static Transform Scale(float s);
        static Transform Normal(void);

        Transform& translate(float x, float y, float z);
        Transform& rotate(float theta, const glm::vec3& axis);
        Transform& scale(float s);

        operator glm::mat4(void) const;

        Transform operator*(const Transform& other) const;
        Transform& operator*=(const Transform& other);
    };
}

#endif // DRK_TRANSFORM_HPP
