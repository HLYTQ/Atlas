#pragma once

#include <entt/entt.hpp>
#include "Entity.hpp"

namespace Atlas {

class Scene {
    friend class Entity;
public:
    Scene();
    ~Scene();

    Entity CreateEntity();

    entt::registry& Reg() {
        return m_Registry;
    }

    void OnUpdate();

private:
    entt::registry m_Registry;
};

} // namespace Atlas
