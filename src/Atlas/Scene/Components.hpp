#pragma once

#include <glm/glm.hpp>
#include "Atlas/Model/Mesh.hpp"

namespace Atlas {

	struct MeshComponent {
    Ref<Mesh> m_Mesh;

    MeshComponent()                           = default;
    MeshComponent(const MeshComponent& other) = default;
    MeshComponent(const Ref<Mesh>& mesh) : m_Mesh(mesh) {}

    operator Ref<Mesh>() {
        return m_Mesh;
    }
};

struct SpriteRendererComponent {
    SpriteRendererComponent()                          = default;
    SpriteRendererComponent(const SpriteRendererComponent&) = default;
    SpriteRendererComponent(const glm::vec4& color) : Color(color) {}

    operator glm::vec4&() {
        return Color;
    }
    operator const glm::vec4&() const {
        return Color;
    }

public:
    glm::vec4 Color;
};

struct TransformComponent {
    TransformComponent()                          = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::mat4& transform) : Transform(transform) {}

    operator glm::mat4&() {
        return Transform;
    }
    operator const glm::mat4&() const {
        return Transform;
    }

public:
    glm::mat4 Transform;
};


} // namespace Atlas
