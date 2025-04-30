#include "Scene.hpp"

#include "atlaspch.hpp"
#include <glm/glm.hpp>

#include "Components.hpp"
#include "Atlas/Renderer/Renderer3D.hpp"
#include "Atlas/Renderer/VertexArray.hpp"

namespace Atlas {

Scene::Scene() {
}

Scene::~Scene() {}

Entity Scene::CreateEntity()
{
    Entity entity(m_Registry.create(), this);
    return entity;
}

void Scene::OnUpdate() {
    auto va = VertexArray::Create();
    auto group = m_Registry.group<MeshComponent>(entt::get<TransformComponent>);
    for (auto entity : group) {
        auto& [mesh, transform] = group.get<MeshComponent, TransformComponent>(entity);
        Renderer3D::RenderMesh(va, mesh.m_Mesh, transform.Transform);
    }
}

} // namespace Atlas
