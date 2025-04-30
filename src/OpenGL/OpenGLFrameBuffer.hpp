#pragma once

#include "Atlas/Renderer/FrameBuffer.hpp"

namespace Atlas {

    class OpenGLFrameBuffer : public FrameBuffer {
    public:
        OpenGLFrameBuffer(uint32_t width, uint32_t height);
        ~OpenGLFrameBuffer();

        void Bind() const;
        void BindIntermediate() const;
        void Unbind() const;

        void Invaidate();
        void Resize(uint32_t width, uint32_t height);

        unsigned int GetColorAttachment() {
            return m_ColorAttachment;
        }
        const uint32_t GetWidth() const {
            return m_Width;
        }
        const uint32_t GetHeight() const {
            return m_Height;
        }

    private:
        uint32_t m_Width;
        uint32_t m_Height;

        unsigned int m_RendererID                   = 0;
        unsigned int textureColorBufferMultiSampled = 0;
        unsigned int rbo                            = 0;
        unsigned int intermediateFBO                = 0;
        unsigned int m_ColorAttachment              = 0;
        // unsigned int m_DepthAttachment = 0;
    };

    class OpenGLFrameBufferHDR : public FrameBufferHDR {
    public:
        OpenGLFrameBufferHDR();
        ~OpenGLFrameBufferHDR();
        void Bind() const override;
        void Unbind() const override;
    private:
        unsigned int m_FBO;
        unsigned int m_RBO;
    };

} // namespace Atlas
