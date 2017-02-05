#include "glare/opengl/vertex_array.hpp"

namespace glare {

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_id);
}

VertexArray::VertexArray(VertexArray&& other) noexcept
  : m_id(other.m_id)
{
    other.m_id = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    if (&other != this) {
        glDeleteVertexArrays(1, &other.m_id);
        m_id = other.m_id;
        other.m_id = 0;
    }
    return *this;
}


VertexArray::~VertexArray() noexcept
{
    glDeleteVertexArrays(1, &m_id);
}

void VertexArray::bind()
{
    glBindVertexArray(m_id);
}

void VertexArray::enable(unsigned location)
{
    bind();
    glEnableVertexAttribArray(location);
}

void VertexArray::disable(unsigned location)
{
    bind();
    glDisableVertexAttribArray(location);
}

} // ns glare
