/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/opengl/renderbuffer.hpp"
#include "glare/opengl/extensions.hpp"

namespace glare {

Renderbuffer::Renderbuffer()
{
    if (ext::has_dsa) {
        glCreateRenderbuffers(1, &m_id);
    } else {
        glGenRenderbuffers(1, &m_id);
    }
}

Renderbuffer::Renderbuffer(Renderbuffer&& other) noexcept
  : m_id(other.m_id)
{
    other.m_id = 0;
}

Renderbuffer& Renderbuffer::operator=(Renderbuffer&& other) noexcept
{
    if (&other != this) {
        glDeleteRenderbuffers(1, &m_id);
        m_id = other.m_id;
        other.m_id = 0;
    }
    return *this;
}

Renderbuffer::~Renderbuffer() noexcept
{
    glDeleteRenderbuffers(1, &m_id);
}

void Renderbuffer::bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_id);
}

void Renderbuffer::allocate_storage(GLenum internal_format,
                                    Size2D size,
                                    unsigned int samples)
{
    if (ext::has_dsa) {
        glNamedRenderbufferStorageMultisample(
            m_id,
            samples,
            internal_format,
            size.width,
            size.height
        );
    } else {
        bind();
        glRenderbufferStorageMultisample(
            GL_RENDERBUFFER,
            samples,
            internal_format,
            size.width,
            size.height
        );
    }
}

} // ns glare
