#include "Camera.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/trigonometric.hpp>

namespace DrkCraft
{
    Camera::Camera(float fov, float aspectRatio, float nearClip, float farClip)
    {
        set_fov(fov);
        set_aspect_ratio(aspectRatio);
        set_near_plane(nearClip);
        set_far_plane(farClip);
        calculate_projection();
    }

    void Camera::set_fov(float fov)
    {
        m_fov = glm::radians(fov);
    }

    void Camera::set_aspect_ratio(float aspectRatio)
    {
        m_aspectRatio = aspectRatio;
    }

    void Camera::set_near_plane(float m_nearClip)
    {
        m_nearClip = m_nearClip;
    }

    void Camera::set_far_plane(float m_farClip)
    {
        m_farClip = m_farClip;
    }

    void Camera::calculate_projection(void)
    {
        m_viewProjection = glm::perspective(m_fov, m_aspectRatio, m_nearClip, m_farClip);
    }

    const glm::mat4& Camera::get_projection_matrix(void) const
    {
        return m_viewProjection;
    }
}
