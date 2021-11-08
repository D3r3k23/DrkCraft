#include "Scene.hpp"

namespace DrkCraft
{
    Entity Scene::add_entity(std::string_view name)
    {
        Entity entity(m_registry.create(), *this);
        return entity;
    }
}
