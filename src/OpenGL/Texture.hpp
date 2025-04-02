#pragma once

#include <string>

namespace Atlas {

class Texture {
public:
    Texture(const std::string& path);
    Texture(const std::string& path, unsigned int format);
    Texture(const std::string& path, unsigned int internalformat, unsigned int format);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

private:
    void LoadFile(const std::string& path, unsigned int internalformat, unsigned int format);

private:
    unsigned int m_TextureID;
    int          m_width;
    int          m_height;
    int          m_nrChannels;
};

} // namespace Atlas
