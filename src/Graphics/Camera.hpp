#ifndef DRK_CAMERA_HPP
#define DRK_CAMERA_HPP

#include "Core/Base.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace DrkCraft
{
    class Camera
    {
    public:
        Camera(float fov, float aspectRatio, float nearClip, float farClip);
        
        void set_fov(float fov); // Degrees
        void set_aspect_ratio(float aspectRatio);
        void set_near_plane(float nearPlane);
        void set_far_plane(float farPlane);

        void calculate_projection(void);
        const glm::mat4& get_projection_matrix(void) const;

    private:
        glm::mat4 m_viewProjection;

        float m_fov; // Radians
        float m_aspectRatio;
        float m_nearClip;
        float m_farClip;
    };
}

#endif // DRK_CAMERA_HPP
