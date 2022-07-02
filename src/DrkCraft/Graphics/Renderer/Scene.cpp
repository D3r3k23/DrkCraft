#include "Scene.hpp"

namespace DrkCraft
{
    RendererScene::RendererScene(void)
      : m_camera()
    {

    }

    void RendererScene::set_camera(const Camera& camera)
    {
        m_camera = camera;
    }

    const Camera& RendererScene::get_camera(void) const
    {
        return m_camera;
    }

    void RendererScene::add_light(Ref<LightSource> light)
    {
        m_lights.push_back(light);
    }

    std::span<const Ref<LightSource>> RendererScene::get_lights(void) const
    {
        return m_lights;
    }

    void RendererScene::reset_lights(void)
    {
        m_lights.clear();
    }
}
