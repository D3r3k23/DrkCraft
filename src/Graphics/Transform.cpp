#include "Transform.hpp"

namespace DrkCraft
{
    Transform::Transform(const glm::mat4& matrix)
      : m_matrix(matrix)
    { }

    Transform Transform::Translation(double x, double y, double z)
    {
        return Transform(glm::mat4(1.0f));
    }

    Transform Transform::Scale(double s)
    {
        return Transform(glm::mat4(1.0f));
    }

    Transform Transform::Rotation(double theta)
    {
        return Transform(glm::mat4(1.0f));
    }

    Transform Transform::Normal(void)
    {
        return Transform(glm::mat4(1.0f));
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
