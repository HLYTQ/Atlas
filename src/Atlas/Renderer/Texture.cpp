#include "Texture.hpp"

#include "Atlas/Renderer/Renderer.hpp"
#include "OpenGL/OpenGLTexture.hpp"

namespace Atlas {

    Ref<Texture2D> Texture2D::Create(const std::string& path) {
        switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLTexture2D>(path);
        case RendererAPI::API::DirectX:
            AT_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported!");
            return nullptr;
        }
        AT_CORE_ASSERT(false, "Unknown Renderer API!");
        return nullptr;
    }

    Ref<TextureCube> TextureCube::Create(const std::string& path) {
        switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLTextureCube>(path);
        case RendererAPI::API::DirectX:
            AT_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported!");
            return nullptr;
        }
        return nullptr;
    }

    Ref<TextureHDR> TextureHDR::Create(const std::string& path) {
        switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLTextureHDR>(path);
        case RendererAPI::API::DirectX:
            AT_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported!");
            return nullptr;
        }
        return nullptr;
    }

} // namespace Atlas
