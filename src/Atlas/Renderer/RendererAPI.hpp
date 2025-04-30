#pragma once

#include "Atlas/Model/Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "atlaspch.hpp"
#include <glm/glm.hpp>

namespace Atlas {

    class Mesh;

    class RendererAPI {
    public:
        enum class API { None = 0, OpenGL = 1, DirectX = 2 };

    public:
        static Scope<RendererAPI> Create();

        virtual void Clear()                                                                                        = 0;
        virtual void SetClearColor(const glm::vec4& color)                                                          = 0;
        virtual void SetViewPort(int width, int height)                                                             = 0;
        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, unsigned int type)                            = 0;
        virtual void DrawMeshs(Ref<VertexArray>& vertexArray, Ref<Mesh>& mesh, glm::mat4& transform = glm::mat4(0)) = 0;
        virtual void DrawCube(Ref<VertexArray>& vertexArray, Ref<Shader>& shader, Ref<TextureCube>& cube)           = 0;
        virtual void DrawCube(Ref<VertexArray>& vertexArray, Ref<Shader>& shader, Ref<TextureHDR>& cube)            = 0;

        static API GetAPI() noexcept {
            return s_API;
        }
        constexpr static void SetAPI(API api) noexcept {
            s_API = api;
        }

    private:
        static API s_API;
    };

} // namespace Atlas
