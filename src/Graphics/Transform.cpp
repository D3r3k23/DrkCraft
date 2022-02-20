#include "Transform.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace DrkCraft
{
    Transform::Transform(void)
      : m_matrix(Identity())
    { }

    Transform::Transform(const mat4& matrix)
      : m_matrix(matrix)
    { }

    Transform Transform::Identity(void)
    {
        return { glm::identity<mat4>() };
    }

    Transform Transform::Translation(float dx, float dy, float dz)
    {
        return Identity().translate(dx, dy, dz);
    }

    Transform Transform::Translation(const vec3& d)
    {
        return Identity().translate(d);
    }

    Transform Transform::Rotation(float theta, const vec3& axis)
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

    Transform& Transform::translate(float dx, float dy, float dz)
    {
        return translate({dx, dy, dz});
    }

    Transform& Transform::translate(const vec3& d)
    {
        m_matrix = glm::translate(m_matrix, d);
        return *this;
    }

    Transform& Transform::rotate(float theta, const vec3& axis)
    {
        m_matrix = glm::rotate(m_matrix, theta, axis);
        return *this;
    }

    Transform& Transform::scale(float s)
    {
        return *this;
    }

    Transform::operator mat4(void) const
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
