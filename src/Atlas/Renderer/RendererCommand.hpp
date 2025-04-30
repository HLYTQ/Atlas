#pragma once

#include "RendererAPI.hpp"


namespace Atlas {

class RendererCommand {
public:
 
    static void Clear() {
        s_RendererAPI->Clear();
    }
    static void SetViewPort(int width, int height) {
        s_RendererAPI->SetViewPort(width, height);
    }

    static void SetClearColor(const glm::vec4& color) {
        s_RendererAPI->SetClearColor(color);
    }

    static void DrawIndexed(const Ref<VertexArray>& vertexArray) {
        s_RendererAPI->DrawIndexed(vertexArray, 0x0004); // GL_TRIANGLES
    }

    static void DrawIndexed(const Ref<VertexArray>& vertexArray, unsigned int type) {
        s_RendererAPI->DrawIndexed(vertexArray, type);
    }

    static void DrawMeshs(Ref<VertexArray>& vertexArray, Ref<Mesh>& mesh, glm::mat4& transform) {
        s_RendererAPI->DrawMeshs(vertexArray, mesh, transform);
    }

    static void DrawCube(Ref<VertexArray>& vertexArray, Ref<Shader>& shader, Ref<TextureCube>& texture) {
        s_RendererAPI->DrawCube(vertexArray, shader, texture);
    }
    static void DrawCube(Ref<VertexArray>& vertexArray, Ref<Shader>& shader, Ref<TextureHDR>& texture) {
        s_RendererAPI->DrawCube(vertexArray, shader, texture);
    }

private:
    static Scope<RendererAPI> s_RendererAPI;
};

} // namespace Atlas
