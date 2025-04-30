#include "FrameBuffer.hpp"

#include "OpengL/OpenGLFrameBuffer.hpp"
#include "Renderer.hpp"

namespace Atlas {

    Ref<FrameBuffer> FrameBuffer::Create(uint32_t width, uint32_t height) {
        switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLFrameBuffer>(width, height);
        case RendererAPI::API::DirectX:
            AT_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported!");
            return nullptr;
        }
        AT_CORE_ASSERT(false, "Unknown Renderer API!");
        return nullptr;
    }

    Ref<FrameBufferHDR> FrameBufferHDR::Create() {
        switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLFrameBufferHDR>();
        case RendererAPI::API::DirectX:
            AT_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported!");
            return nullptr;
        }
        AT_CORE_ASSERT(false, "Unknown Renderer API!");
        return nullptr;
    }

} // namespace Atlas
