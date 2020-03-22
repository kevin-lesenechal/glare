/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/opengl/vertex_array.hpp"

namespace glare {

VertexArray::VertexArray()
{
    if (ext::has_dsa) {
        glCreateVertexArrays(1, &m_id);
    } else {
        glGenVertexArrays(1, &m_id);
    }
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

void VertexArray::unbind()
{
    glBindVertexArray(0);
}

void VertexArray::enable(unsigned location)
{
    if (ext::has_dsa) {
        glEnableVertexArrayAttrib(m_id, location);
    } else {
        bind();
        glEnableVertexAttribArray(location);
        unbind();
    }
}

void VertexArray::disable(unsigned location)
{
    if (ext::has_dsa) {
        glDisableVertexArrayAttrib(m_id, location);
    } else {
        bind();
        glDisableVertexAttribArray(location);
        unbind();
    }
}

} // ns glare
