#include "VertexArray.hpp"

namespace Atlas {

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_renderID);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_renderID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    Bind();
    vb.Bind();
    const auto& elements = layout.get_elements();
    unsigned int offset  = 0;
    for (int i = 0; i < elements.size(); ++i) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
            i, element.count, element.type, element.normalized, layout.get_stride(), (const void*) offset);
        offset += element.count * VertexBufferElement::get_element_type(element.type);
    }
}

void VertexArray::Bind() const {
    glBindVertexArray(m_renderID);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

} // namespace Atlas
