#include "VertexArray.hpp"

#include "Atlas/Renderer/Renderer.hpp"
#include "OpenGL/OpenGLVertexArray.hpp"
#include "atlaspch.hpp"

namespace Atlas {

Ref<VertexArray> VertexArray::Create() {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported! ")
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLVertexArray>();
    case RendererAPI::API::DirectX:
        AT_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported! ")
        return nullptr;
    }

    AT_CORE_ASSERT(false, "Unknown Renderer API!");
    return nullptr;
}

} // namespace Nut
