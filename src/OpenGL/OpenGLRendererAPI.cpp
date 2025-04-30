#include "OpenGLRendererAPI.hpp"

#include <glad/glad.h>

#include "Atlas/Renderer/Material.hpp"
#include "atlaspch.hpp"

namespace Atlas {

    void OpenGLRendererAPI::Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::SetViewPort(int width, int height) {
        glViewport(0, 0, width, height);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, unsigned int type) {
        glDrawElements(type, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRendererAPI::DrawMeshs(Ref<VertexArray>& vertexArray, Ref<Mesh>& mesh, glm::mat4& transform) {
        glDepthFunc(GL_LESS);
        vertexArray->AddVertexBuffer(mesh->GetVertexBuffer());
        vertexArray->SetIndexBuffer(mesh->GetIndexBuffer());
        vertexArray->Bind();

        auto& materials = mesh->GetMaterials();
        for (Submesh& submesh : mesh->GetSubmeshes()) {
            // Material
            auto material = materials[submesh.MaterialIndex];
            auto shader   = material->GetShader();
            material->UpdateForRendering();

            auto transformUniform = transform * submesh.Transform;
            shader->SetMat4("Transform", transformUniform);
        
            if (material->GetFlag(MaterialFlag::DepthTest)) {
                glEnable(GL_DEPTH_TEST);
            } else {
                glDisable(GL_DEPTH_TEST);
            }
            unsigned int type;
            if (material->GetFlag(MaterialFlag::Lines)) {
                type = GL_LINES;
            } else {
                type = GL_TRIANGLES;
            }
            glDrawElementsBaseVertex(type, submesh.IndexCount, GL_UNSIGNED_INT,
                (void*) (sizeof(uint32_t) * submesh.BaseIndex), submesh.BaseVertex);
        }
    }

    void OpenGLRendererAPI::DrawCube(Ref<VertexArray>& vertexArray, Ref<Shader>& shader, Ref<TextureCube>& cube) {
        glDepthFunc(GL_LEQUAL);
        shader->Bind();
        vertexArray->Bind();
        cube->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);
    }
    void OpenGLRendererAPI::DrawCube(Ref<VertexArray>& vertexArray, Ref<Shader>& shader, Ref<TextureHDR>& cube) {
        // glDepthFunc(GL_LEQUAL);
        shader->Bind();
        vertexArray->Bind();
        cube->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // glDepthFunc(GL_LESS);
    }

} // namespace Atlas
