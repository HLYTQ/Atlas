#pragma once

#include "Atlas/Renderer/RendererAPI.hpp"

namespace Atlas {

    class OpenGLRendererAPI : public RendererAPI {
    public:
        void Clear() override;
        void SetClearColor(const glm::vec4& color) override;
        void SetViewPort(int width, int height) override;
        void DrawIndexed(const Ref<VertexArray>& vertexArray, unsigned int type) override;
        void DrawMeshs(Ref<VertexArray>& vertexArray, Ref<Mesh>& mesh, glm::mat4& transform = glm::mat4(0)) override;
        void DrawCube(Ref<VertexArray>& vertexArray, Ref<Shader>& shader, Ref<TextureCube>& cube) override;
        void DrawCube(Ref<VertexArray>& vertexArray, Ref<Shader>& shader, Ref<TextureHDR>& cube) override;
    };

} // namespace Atlas
