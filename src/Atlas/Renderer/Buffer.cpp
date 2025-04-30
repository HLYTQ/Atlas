#include "Buffer.hpp"

#include "OpenGL/OpenGLBuffer.hpp"
#include "RendererAPI.hpp"
#include "atlaspch.hpp"

namespace Atlas {

    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
        switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:
            AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported! ");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLVertexBuffer>(vertices, size);
        case RendererAPI::API::DirectX:
            AT_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported! ");
            return nullptr;
        }

        AT_CORE_ASSERT(false, "Unknown Renderer API!");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
        switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:
            AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported! ");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLIndexBuffer>(indices, count);
        case RendererAPI::API::DirectX:
            AT_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported! ");
            return nullptr;
        }

        AT_CORE_ASSERT(false, "Unknown Renderer API!");
        return nullptr;
    }
} // namespace Atlas
