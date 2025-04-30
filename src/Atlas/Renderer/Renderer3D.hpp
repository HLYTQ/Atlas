#pragma once

#include "Atlas/Camera/EditorCamera.hpp"
#include "Atlas/Model/Mesh.hpp"
#include "Atlas/Renderer/VertexArray.hpp"
#include "Atlas/Scene/Scene.hpp"
#include <glm/glm.hpp>

namespace Atlas {
    class Mesh;

    class Renderer3D {
    public:
        static void Init();
        static void ShutDown();

        static void BeginScene(const Scene* Scene, const EditorCamera& camera);
        static void EndScene();
        static void SetMesh(Ref<Mesh> mesh, const glm::mat4 transform = glm::mat4(1.0));
        static void RenderMesh(Ref<VertexArray>& vertexArray, Ref<Mesh>& mesh, const glm::mat4& transform);
        static void RenderCube(Ref<Shader>& shader, Ref<TextureCube>& texture);
        static void RenderCube(Ref<Shader>& shader, Ref<TextureHDR>& texture);
    };

} // namespace Atlas
