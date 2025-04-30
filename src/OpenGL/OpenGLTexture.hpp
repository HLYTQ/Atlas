#pragma once

#include <array>

#include "Atlas/Renderer/Texture.hpp"

namespace Atlas {

    class OpenGLTexture2D : public Texture2D {
    public:
        OpenGLTexture2D(const std::string& path);
        ~OpenGLTexture2D();

        void Bind(unsigned int slot = 0) const override;

        unsigned int GetRendererID() const override {
            return m_RendererID;
        }

        unsigned int GetWidth() const override {
            return m_Width;
        }
        unsigned int GetHeight() const override {
            return m_Height;
        }

        const bool Loaded() noexcept override {
            return m_Isloaded;
        }

    private:
        std::string m_Path;

        unsigned int m_Width;
        unsigned int m_Height;
        unsigned int m_RendererID;

        bool m_Isloaded = false;
    };

    class OpenGLTextureCube : public TextureCube {
        static constexpr std::array faces = {"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"};

    public:
        OpenGLTextureCube(const std::string& path);
        ~OpenGLTextureCube();

        void Bind(unsigned int slot = 0) const;

        unsigned int GetRendererID() const override {
            return m_RendererID;
        }

        unsigned int GetWidth() const override {
            return m_Width;
        }

        unsigned int GetHeight() const override {
            return m_Height;
        }

        const bool Loaded() noexcept override {
            return m_Isloaded;
        }

    private:
        int m_Width;
        int m_Height;
        int m_NrChannels;

        unsigned int m_RendererID;
        bool m_Isloaded = false;
    };


    class OpenGLTextureHDR : public TextureHDR {
    public:
        OpenGLTextureHDR(const std::string& path);
        ~OpenGLTextureHDR();

        void Bind(unsigned int slot = 0) const;

        unsigned int GetRendererID() const override {
            return m_RendererID;
        }

        unsigned int GetWidth() const override {
            return m_Width;
        }

        unsigned int GetHeight() const override {
            return m_Height;
        }

        const bool Loaded() noexcept override {
            return m_Isloaded;
        }

    private:
        int m_Width;
        int m_Height;
        int m_NrChannels;

        unsigned int m_RendererID;
        bool m_Isloaded = false;
    };

} // namespace Atlas
