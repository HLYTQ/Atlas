#pragma once

#include <glad/glad.h>
#include <stdexcept>
#include <vector>

namespace Atlas {

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int get_element_type(const unsigned int type) {
        switch (type) {
        case GL_FLOAT:
            return 4;
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        default:
            throw std::runtime_error("unexcepted GL type.");
            return -1;
        }
    }
};

struct VertexBufferLayout {
public:
    VertexBufferLayout() : m_stride(0) {}

    void push_float(unsigned int count) {
        m_element.emplace_back(VertexBufferElement{GL_FLOAT, count, GL_FALSE});
        m_stride += 4 * count;
    }

    void push_uint(unsigned int count) {
        m_element.emplace_back(VertexBufferElement{GL_UNSIGNED_INT, count, GL_FALSE});
        m_stride += 4 * count;
    }

    void push_byte(unsigned int count) {
        m_element.emplace_back(VertexBufferElement{GL_BYTE, count, GL_TRUE});
        m_stride += count;
    }

    const std::vector<VertexBufferElement>& get_elements() const noexcept {
        return m_element;
    }
    unsigned int get_stride() const noexcept {
        return m_stride;
    }

private:
    std::vector<VertexBufferElement> m_element;
    unsigned int m_stride;
};

} // namespace Atlas
