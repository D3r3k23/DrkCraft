#include "Transform.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace DrkCraft
{
    Transform::Transform(void)
      : m_matrix(Identity())
    { }

    Transform::Transform(const glm::mat4& matrix)
      : m_matrix(matrix)
    { }

    Transform Transform::Identity(void)
    {
        return { glm::identity<glm::mat4>() };
    }

    Transform Transform::Translation(float x, float y, float z)
    {
        return Identity().translate(x, y, z);
    }

    Transform Transform::Rotation(float theta, const glm::vec3& axis)
    {
        return Identity().rotate(theta, axis);
    }

    Transform Transform::Scale(float s)
    {
        return Identity();
    }

    Transform Transform::Normal(void)
    {
        return Identity();
    }

    Transform& Transform::translate(float x, float y, float z)
    {
        m_matrix = glm::translate(m_matrix, {x, y, z});
        return *this;
    }

    Transform& Transform::rotate(float theta, const glm::vec3& axis)
    {
        m_matrix = glm::rotate(m_matrix, theta, axis);
        return *this;
    }

    Transform& Transform::scale(float s)
    {
        return *this;
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
