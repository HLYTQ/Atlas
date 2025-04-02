#pragma once

namespace Atlas {

class IndexBuffer {
public:
    IndexBuffer(unsigned int count, const unsigned int* data);
    ~IndexBuffer();
    void         Bind() const;
    void         Unbind() const;
    unsigned int GetCount() const { return m_Count; }

private:
    unsigned int m_RenderID;
    unsigned int m_Count;
};

} // namespace Atlas
