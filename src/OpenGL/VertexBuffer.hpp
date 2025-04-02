#pragma once

namespace Atlas {

class VertexBuffer {
public:
    VertexBuffer(unsigned int size, const void* data);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

private:
    unsigned int m_RenderID;
};

} // namespace Atlas
