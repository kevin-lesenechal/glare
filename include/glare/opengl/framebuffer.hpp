/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <epoxy/gl.h>

namespace glare {

class Texture;
class Renderbuffer;

class Framebuffer
{
public:
    enum class Target : GLenum {
        DrawOnly    = GL_DRAW_FRAMEBUFFER,
        ReadOnly    = GL_READ_FRAMEBUFFER,
        ReadDraw    = GL_FRAMEBUFFER
    };

    enum class Attachment : GLenum {
        Color0          = GL_COLOR_ATTACHMENT0,
        Depth           = GL_DEPTH_ATTACHMENT,
        Stencil         = GL_STENCIL_ATTACHMENT,
        DepthStencil    = GL_DEPTH_STENCIL_ATTACHMENT
    };

    explicit Framebuffer(Target target);

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;

    Framebuffer(Framebuffer&& other) noexcept;
    Framebuffer& operator=(Framebuffer&& other) noexcept;

    ~Framebuffer() noexcept;

    void bind();
    void unbind();

    void attach(const Texture& texture,
                Attachment attachment,
                unsigned mipmap_level = 0);

    void attach(const Renderbuffer& renderbuffer, Attachment attachment);

    void ensure_complete();

private:
    GLuint m_id;
    Target m_target;
};

} // ns glare
