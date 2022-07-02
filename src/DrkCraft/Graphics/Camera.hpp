#ifndef DRK_GRAPHICS_CAMERA_HPP
#define DRK_GRAPHICS_CAMERA_HPP

#include "Core/Base.hpp"

#include "Lib/glm/vec3.hpp"
#include "Lib/glm/mat4.hpp"

namespace DrkCraft
{
    class Camera
    {
    public:
        Camera(float fov, float aspectRatio, float nearClip, float farClip,
            const vec3& position, const vec3& direction, const vec3& vNeverParallel={0.0f, 1.0f, 0.0f});

        Camera(void);

        void set_fov(float fov); // Degrees
        void set_aspect_ratio(float aspectRatio);
        void set_near_plane(float nearPlane);
        void set_far_plane(float farPlane);

        void set_position(const vec3& position);
        void set_direction(const vec3& direction);

        void move(const vec3& offset);
        void rotate(float angle);
        void tilt(float angle);

        const mat4& get_view_projection(void) const;

    private:
        void calculate_projection(void);
        void calculate_view_projection(void);

    private:
        float m_fov; // Radians
        float m_aspectRatio;
        float m_nearClip;
        float m_farClip;

        vec3 m_position;
        vec3 m_direction;

        vec3 m_vNeverParallel;

        mat4 m_projection;
        mat4 m_viewProjection;
    };
}

#endif // DRK_GRAPHICS_CAMERA_HPP
