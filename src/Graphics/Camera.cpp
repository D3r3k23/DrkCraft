#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace DrkCraft
{
    Camera::Camera(float fov, float aspectRatio, float nearClip, float farClip,
        const vec3& position, const vec3& direction, const vec3& vNeverParallel)
      : m_fov(glm::radians(fov)),
        m_aspectRatio(aspectRatio),
        m_nearClip(nearClip),
        m_farClip(farClip),
        m_position(position),
        m_direction(direction),
        m_vNeverParallel(vNeverParallel)
    {
        calculate_projection();
        calculate_view_projection();
    }

    void Camera::set_fov(float fov)
    {
        m_fov = glm::radians(fov);
        calculate_projection();
    }

    void Camera::set_aspect_ratio(float aspectRatio)
    {
        m_aspectRatio = aspectRatio;
        calculate_projection();
    }

    void Camera::set_near_plane(float nearClip)
    {
        m_nearClip = nearClip;
        calculate_projection();
    }

    void Camera::set_far_plane(float farClip)
    {
        m_farClip = farClip;
        calculate_projection();
    }

    void Camera::set_position(const vec3& position)
    {
        m_position = position;
        calculate_view_projection();
    }

    void Camera::set_direction(const vec3& direction)
    {
        m_direction = direction;
        calculate_view_projection();
    }

    void Camera::move(const vec3& offset)
    {
        m_position += offset;
        calculate_view_projection();
    }

    void Camera::rotate(float angle)
    {

        calculate_view_projection();
    }

    void Camera::tilt(float angle)
    {

        calculate_view_projection();
    }

    const mat4& Camera::get_view_projection(void) const
    {
        return m_viewProjection;
    }

    void Camera::calculate_projection(void)
    {
        m_projection = glm::perspective(m_fov, m_aspectRatio, m_nearClip, m_farClip);
    }

    void Camera::calculate_view_projection(void)
    {
        const auto view = glm::lookAt(m_position, m_direction, {0.0f, 1.0f, 0.0f});
        m_viewProjection = m_projection * view;
    }
}
