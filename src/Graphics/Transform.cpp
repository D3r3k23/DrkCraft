#include "Transform.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace DrkCraft
{
    Transform::Transform(void)
      : m_matrix(glm::identity<glm::mat4>())
    { }

    Transform::Transform(const glm::mat4& matrix)
      : m_matrix(matrix)
    { }

    Transform Transform::Translation(float x, float y, float z)
    {
        return { glm::translate(glm::identity<glm::mat4>(), glm::vec3(x, y, z)) };
    }

    Transform Transform::Scale(float s)
    {
        return { glm::mat4(1.0f) };
    }

    Transform Transform::Rotation(float theta, const glm::vec3& axis)
    {
        return { glm::rotate(glm::identity<glm::mat4>(), theta, axis) };
    }

    Transform Transform::Normal(void)
    {
        return {glm::mat4(1.0f)};
    }

    Transform::operator glm::mat4(void) const
    {
        return m_matrix;
    }

    Transform Transform::operator*(const Transform& other) const
    {
        return m_matrix * other.m_matrix;
    }

    Transform& Transform::operator*=(const Transform& other)
    {
        *this = *this * other;
        return *this;
    }
}
