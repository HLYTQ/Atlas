#pragma once

#include "RendererCommand.hpp"
#include "Atlas/Model/Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace Atlas {

class Renderer {
public:
    static void BeginScene();
    static void EndScene();

    static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
    static void SubmitMesh(Ref<VertexArray>& vertexArray, Ref<Mesh> mesh, glm::mat4 transform = glm::mat4(1.0f));
    static void SubmitCube(Ref<VertexArray>& vertexArray, Ref<Shader>& shader, Ref<TextureCube>& texture);
    static void SubmitCube(Ref<VertexArray>& vertexArray, Ref<Shader>& shader, Ref<TextureHDR>& texture);
    inline static RendererAPI::API GetAPI() {
        return RendererAPI::GetAPI();
    }
private:
};

} // namespace Atlas
