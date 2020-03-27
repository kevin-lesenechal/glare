/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <epoxy/gl.h>

#include "glare/sizes.hpp"

namespace glare {

class Renderbuffer
{
public:
    Renderbuffer();

    Renderbuffer(const Renderbuffer&) = delete;
    Renderbuffer operator=(const Renderbuffer&) = delete;

    Renderbuffer(Renderbuffer&& other) noexcept;
    Renderbuffer& operator=(Renderbuffer&& other) noexcept;

    ~Renderbuffer() noexcept;

    void bind();

    void allocate_storage(GLenum internal_format,
                          Size2D size,
                          unsigned samples = 0);

    [[nodiscard]] GLuint id() const noexcept { return m_id; }

private:
    GLuint m_id;
};

} // ns glare
