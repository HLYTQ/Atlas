#include "Renderer.hpp"

#include "atlaspch.hpp"

namespace Atlas {

    void Renderer::BeginScene() {}

    void Renderer::EndScene() {}

    void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray) {
        vertexArray->Bind();
        RendererCommand::DrawIndexed(vertexArray);
    }

    void Renderer::SubmitMesh(Ref<VertexArray>& vertexArray, Ref<Mesh> mesh, glm::mat4 transform) {
        RendererCommand::DrawMeshs(vertexArray, mesh, transform);
    }

    void Renderer::SubmitCube(Ref<VertexArray>& vertexArray, Ref<Shader>& shader, Ref<TextureCube>& texture) {
        RendererCommand::DrawCube(vertexArray, shader, texture);
    }

    void Renderer::SubmitCube(Ref<VertexArray>& vertexArray, Ref<Shader>& shader, Ref<TextureHDR>& texture) {
        RendererCommand::DrawCube(vertexArray, shader, texture);
    }


} // namespace Atlas
