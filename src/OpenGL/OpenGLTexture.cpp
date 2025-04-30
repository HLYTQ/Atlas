#include "OpenGLTexture.hpp"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Atlas {

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path) {
        stbi_set_flip_vertically_on_load(1);

        int width, height, channels;
        stbi_uc* data = nullptr;
        {
            data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        }
        AT_CORE_TRACE("Which is from : {0}", path);
        AT_CORE_ASSERT(data, "Failed to load image!");
        m_Width  = width;
        m_Height = height;

        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 3) {
            internalFormat = GL_RGB8;
            dataFormat     = GL_RGB;
        } else if (channels == 4) {
            internalFormat = GL_RGBA8;
            dataFormat     = GL_RGBA;
        }
        AT_CORE_ASSERT((internalFormat & dataFormat), "Format not supported!");
        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        stbi_image_free(data);
        m_Isloaded = true;
    }

    OpenGLTexture2D::~OpenGLTexture2D() {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::Bind(unsigned int slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }

    OpenGLTextureCube::OpenGLTextureCube(const std::string& path) {
        stbi_set_flip_vertically_on_load(0);
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

        unsigned char* data;
        GLenum internalFormat = 0, dataFormat = 0;
        for (unsigned int i = 0; i < faces.size(); ++i) {
            data = stbi_load((path + std::string(faces[i])).c_str(), &m_Width, &m_Height, &m_NrChannels, 0);
            if (m_NrChannels == 3) {
                internalFormat = GL_RGB8;
                dataFormat     = GL_RGB;
            } else if (m_NrChannels == 4) {
                internalFormat = GL_RGBA8;
                dataFormat     = GL_RGBA;
            }
            AT_CORE_ASSERT((internalFormat & dataFormat), "Format not supported!");
            if (data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, dataFormat, m_Width, m_Height, 0, dataFormat,
                    GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            } else {
                AT_CORE_ASSERT(false, "Cubemap texture failed to load at path");
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    OpenGLTextureCube::~OpenGLTextureCube() {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTextureCube::Bind(unsigned int slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
    }

    OpenGLTextureHDR::OpenGLTextureHDR(const std::string& path) {
        stbi_set_flip_vertically_on_load(1);
        float* data = stbi_loadf(path.c_str(), &m_Width, &m_Height, &m_NrChannels, 0);
        if (data) {
            glGenTextures(1, &m_RendererID);
            glBindTexture(GL_TEXTURE_2D, m_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        } else {
            AT_CORE_ASSERT(false, "HDR texture failed to load at path");
        }
    }

    OpenGLTextureHDR::~OpenGLTextureHDR() {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTextureHDR::Bind(unsigned int slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }



} // namespace Atlas
