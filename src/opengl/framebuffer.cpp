/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include <stdexcept>
#include "glare/opengl/framebuffer.hpp"
#include "glare/opengl/renderbuffer.hpp"
#include "glare/opengl/extensions.hpp"
#include "glare/material/texture.hpp"

namespace glare {

Framebuffer::Framebuffer(Target target)
  : m_target(target)
{
    if (ext::has_dsa) {
        glCreateFramebuffers(1, &m_id);
    } else {
        glGenFramebuffers(1, &m_id);
    }
}

Framebuffer::Framebuffer(Framebuffer&& other) noexcept
  : m_id(other.m_id),
    m_target(other.m_target)
{
    other.m_id = 0;
}

Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept
{
    if (&other != this) {
        glDeleteFramebuffers(1, &m_id);
        m_id = other.m_id;
        other.m_id = 0;
        m_target = other.m_target;
    }
    return *this;
}

Framebuffer::~Framebuffer() noexcept
{
    glDeleteFramebuffers(1, &m_id);
}

void Framebuffer::bind()
{
    glBindFramebuffer(static_cast<GLenum>(m_target), m_id);
}

void Framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::attach(const Texture& texture,
                         Attachment attachment,
                         unsigned mipmap_level)
{
    if (ext::has_dsa) {
        glNamedFramebufferTexture(
            m_id,
            static_cast<GLenum>(attachment),
            texture.id(),
            mipmap_level
        );
    } else {
        bind();
        glFramebufferTexture(
            static_cast<GLenum>(m_target),
            static_cast<GLenum>(attachment),
            texture.id(),
            mipmap_level
        );
    }
}

void Framebuffer::attach(const Renderbuffer& renderbuffer,
                         Framebuffer::Attachment attachment)
{
    if (ext::has_dsa) {
        glNamedFramebufferRenderbuffer(
            m_id,
            static_cast<GLenum>(attachment),
            GL_RENDERBUFFER,
            renderbuffer.id()
        );
    } else {
        bind();
        glFramebufferRenderbuffer(
            static_cast<GLenum>(m_target),
            static_cast<GLenum>(attachment),
            GL_RENDERBUFFER,
            renderbuffer.id()
        );
    }
}

void Framebuffer::ensure_complete()
{
    GLenum status;

    if (ext::has_dsa) {
        status = glCheckNamedFramebufferStatus(
            m_id,
            static_cast<GLenum>(m_target)
        );
    } else {
        bind();
        status = glCheckFramebufferStatus(static_cast<GLenum>(m_target));
    }

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error(
            "Framebuffer incomplete:" + std::to_string(status)
        );
    }
}

void Framebuffer::set_draw_buffer(unsigned color_index)
{
    if (ext::has_dsa) {
        glNamedFramebufferDrawBuffer(m_id, GL_COLOR_ATTACHMENT0 + color_index);
    } else {
        bind();
        glDrawBuffer(GL_COLOR_ATTACHMENT0 + color_index);
    }
}

void Framebuffer::set_color_read_buffer(unsigned color_index)
{
    if (ext::has_dsa) {
        glNamedFramebufferReadBuffer(m_id, GL_COLOR_ATTACHMENT0 + color_index);
    } else {
        bind();
        glReadBuffer(GL_COLOR_ATTACHMENT0 + color_index);
    }
}

void Framebuffer::disable_draw_buffer()
{
    if (ext::has_dsa) {
        glNamedFramebufferDrawBuffer(m_id, GL_NONE);
    } else {
        bind();
        glDrawBuffer(GL_NONE);
    }
}

void Framebuffer::disable_color_read_buffer()
{
    if (ext::has_dsa) {
        glNamedFramebufferReadBuffer(m_id, GL_NONE);
    } else {
        bind();
        glReadBuffer(GL_NONE);
    }
}

} // ns glare
