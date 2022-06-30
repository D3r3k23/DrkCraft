#include "Scene.hpp"

namespace DrkCraft
{
    Scene::Scene(void)
      : m_camera()
    {

    }

    void Scene::set_camera(const Camera& camera)
    {
        m_camera = camera;
    }

    const Camera& Scene::get_camera(void) const
    {
        return m_camera;
    }

    void Scene::add_light(Ref<LightSource> light)
    {
        m_lights.push_back(light);
    }

    std::span<const Ref<LightSource>> Scene::get_lights(void) const
    {
        return m_lights;
    }

    void Scene::reset_lights(void)
    {
        m_lights.clear();
    }
}
