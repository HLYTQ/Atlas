#include "Renderer3D.hpp"

#include <glad/glad.h>
#include <vector>

// #include "Atlas/Model/Model.hpp"
#include "Atlas/Model/Mesh.hpp"
#include "Atlas/Renderer/Material.hpp"
#include "Atlas/Renderer/Renderer.hpp"
#include "Atlas/Scene/Entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Atlas {

    static std::string GetRootPath(const std::string& path) noexcept {
        return std::string(AT_CURRENT_PATH) + path;
    }

    static float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    struct Renderer3DStorage {
        Ref<Mesh> ModelMesh;
        Ref<VertexArray> BoxVertexArray;
        Ref<VertexBuffer> BoxVertexBuffer;
        BufferLayout BoxBufferLayout;
        const Scene* ActiveScene = nullptr;
    };
    static Renderer3DStorage* s_Data;

    void Renderer3D::Init() {
        s_Data                 = new Renderer3DStorage();
        s_Data->BoxVertexArray = VertexArray::Create();
        s_Data->BoxVertexBuffer = VertexBuffer::Create(skyboxVertices, sizeof(skyboxVertices));
        s_Data->BoxBufferLayout = BufferLayout {
            { ShaderDataType::Float3, "aPos" },
        };
        s_Data->BoxVertexBuffer->SetLayout(s_Data->BoxBufferLayout);
        s_Data->BoxVertexArray->AddVertexBuffer(s_Data->BoxVertexBuffer);

        // s_Data->ModelMesh = Atlas::CreateRef<Atlas::Mesh>(GetRootPath("assets/Models/78040/78040.fbx"));
    }

    void Renderer3D::BeginScene(const Scene* Scene, const EditorCamera& camera) {
        s_Data->ActiveScene = Scene;

        glm::mat4 viewProjection = camera.GetViewProjection();
        s_Data->ModelMesh->m_MeshShader->SetMat4("viewProjection", viewProjection);
    }

    void Renderer3D::EndScene() {}

    void Renderer3D::SetMesh(Ref<Mesh> mesh, const glm::mat4 transform) {
        s_Data->ModelMesh = mesh;
    }

    void Renderer3D::RenderMesh(Ref<VertexArray>& vertexArray, Ref<Mesh>& mesh, const glm::mat4& transform) {
        Renderer::SubmitMesh(vertexArray, mesh, transform);
    }

    void Renderer3D::RenderCube(Ref<Shader>& shader, Ref<TextureCube>& texture) {
        Renderer::SubmitCube(s_Data->BoxVertexArray, shader, texture);
    }

    void Renderer3D::RenderCube(Ref<Shader>& shader, Ref<TextureHDR>& texture) {
        Renderer::SubmitCube(s_Data->BoxVertexArray, shader, texture);
    }

    void Renderer3D::ShutDown() {
        delete s_Data;
    }

} // namespace Atlas
