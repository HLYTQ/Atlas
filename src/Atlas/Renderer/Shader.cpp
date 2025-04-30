#include "Shader.hpp"

#include "glad/glad.h"
#include "atlaspch.hpp"
#include "Atlas/Renderer/Renderer.hpp"
#include "OpenGL/OpenGLShader.hpp"

namespace Atlas {

Ref<Shader> Shader::Create(const std::string& filepath) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLShader>(filepath);
    case RendererAPI::API::DirectX:
        AT_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported!");
        return nullptr;
    }
    AT_CORE_ASSERT(false, "Unknown Renderer API!");
    return nullptr;
}

Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
    case RendererAPI::API::DirectX:
        AT_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported!");
        return nullptr;
    }

    AT_CORE_ASSERT(false, "Unknown Renderer API!");
    return nullptr;
}

} // namespace Atlas
