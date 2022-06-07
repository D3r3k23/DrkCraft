#ifndef DRK_GRAPHICS_SCENE_HPP
#define DRK_GRAPHICS_SCENE_HPP

#include "Core/Base.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/LightSource.hpp"

#include <vector>
#include <span>

namespace DrkCraft
{
    class Scene
    {
    public:
        Scene(void);

        void set_camera(const Camera& camera);
        const Camera& get_camera(void) const;

        void add_light(Ref<LightSource> light);
        std::span<const Ref<LightSource>> get_lights(void) const;

        void reset_lights(void);

    private:
        Camera m_camera;

        std::vector<Ref<LightSource>> m_lights;
    };
}

#endif // DRK_GRAPHICS_SCENE_HPP
