#include "RendererAPI.hpp"

#include "atlaspch.hpp"
#include "OpenGL/OpenGLRendererAPI.hpp"

namespace Atlas {

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

Scope<RendererAPI> RendererAPI::Create() {

    switch (RendererAPI::GetAPI()) {
    case RendererAPI::API::None:
        AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateScope<OpenGLRendererAPI>();
    case RendererAPI::API::DirectX:
        AT_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported!");
        return nullptr;
    }
    AT_CORE_ASSERT(false, "Unknown Renderer API!");
    return nullptr;
}

}
