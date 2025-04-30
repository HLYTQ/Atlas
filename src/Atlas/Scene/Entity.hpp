#pragma once

#include "Scene.hpp"
#include "Atlas/Log.hpp"

namespace Atlas {

class Scene;

class Entity {
public:
    Entity() = default;
    Entity(const Entity&) = default;
    Entity(entt::entity handle, Scene* scene);

    template <typename T, typename... Args>
    T& AddComponent(Args&&... args) {
        AT_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
        return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
    }

    template <typename T>
    T& GetComponent() {
        AT_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
        return m_Scene->m_Registry.get<T>(m_EntityHandle);
    }

    template <typename T>
    bool HasComponent() {
        return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
    }

    template <typename T>
    void RemoveComponent() {
        AT_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
        m_Scene->m_Registry.remove<T>(m_EntityHandle);
    }

    operator bool() const {
        return m_EntityHandle != entt::null;
    }

private:
    entt::entity m_EntityHandle;
    Scene* m_Scene;
};

} // namespace Atlas
