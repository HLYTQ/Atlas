#include "OpenGLFrameBuffer.hpp"

#include <glad/glad.h>

namespace Atlas {

    constexpr static uint32_t MAX_FRAMEBUFFER_SIZE = 8192;

    OpenGLFrameBuffer::OpenGLFrameBuffer(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {
        Invaidate();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer() {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteFramebuffers(1, &rbo);
        glDeleteFramebuffers(1, &textureColorBufferMultiSampled);
        glDeleteFramebuffers(1, &intermediateFBO);
        glDeleteFramebuffers(1, &m_ColorAttachment);
        // glDeleteFramebuffers(1, &m_DepthAttachment);
    }

    void OpenGLFrameBuffer::Invaidate() {
        if (m_RendererID) {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteFramebuffers(1, &rbo);
            glDeleteFramebuffers(1, &textureColorBufferMultiSampled);
            glDeleteFramebuffers(1, &intermediateFBO);
            glDeleteFramebuffers(1, &m_ColorAttachment);
            // glDeleteFramebuffers(1, &m_DepthAttachment);
        }

        // 创建缓冲区
        glGenFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        glGenTextures(1, &textureColorBufferMultiSampled);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, m_Width, m_Height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);

        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, m_Width, m_Height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            AT_CORE_ASSERT(false, "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenFramebuffers(1, &intermediateFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

        // 生成三颜色通道纹理
        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // 生成对应的颜色缓冲区
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

        // 生成深度纹理
        // glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
        // glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
        // glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Width, m_Height);

        // 生成深度缓冲区
        // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
        // if (!(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)) {
        //     AT_CORE_ASSERT(false, "FrameBuffer Compile ERROR!");
        // }
        //
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


    void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height) {
        if (width == 0 || height == 0 || width > MAX_FRAMEBUFFER_SIZE || height > MAX_FRAMEBUFFER_SIZE) {
            AT_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
            return;
        }
        m_Width  = width;
        m_Height = height;

        Invaidate();
    }

    void OpenGLFrameBuffer::Bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        glViewport(0, 0, m_Width, m_Height);
    }

    void OpenGLFrameBuffer::BindIntermediate() const {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
        glBlitNamedFramebuffer(m_RendererID, intermediateFBO, 0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height,
            GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }

    void OpenGLFrameBuffer::Unbind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    OpenGLFrameBufferHDR::OpenGLFrameBufferHDR() {
        glGenFramebuffers(1, &m_FBO);
        glGenFramebuffers(1, &m_RBO);

        
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO);  
        
    }

    OpenGLFrameBufferHDR::~OpenGLFrameBufferHDR() {
        glDeleteFramebuffers(1, &m_RBO);
        glDeleteFramebuffers(1, &m_FBO);
    }

    void OpenGLFrameBufferHDR::Bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    }

    void OpenGLFrameBufferHDR::Unbind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

} // namespace Atlas
