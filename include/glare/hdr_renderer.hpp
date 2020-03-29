/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include "glare/opengl/framebuffer.hpp"
#include "glare/material/texture.hpp"
#include "glare/opengl/renderbuffer.hpp"

namespace glare {

class ShaderProgram;

class HdrRenderer
{
public:
    HdrRenderer(ShaderProgram& program, Size2D size, unsigned nr_samples = 0);

    void start_scene();
    void end_scene_draw();
    void resize(Size2D new_size);

    HdrRenderer(const HdrRenderer&) = delete;
    HdrRenderer& operator=(const HdrRenderer&) = delete;
    HdrRenderer(HdrRenderer&&) noexcept = default;
    HdrRenderer& operator=(HdrRenderer&&) noexcept = default;

private:
    void setup_buffers(Texture& color_buffer,
                       Renderbuffer& depth_buffer,
                       Size2D size);

private:
    Framebuffer     m_framebuffer;
    Texture         m_color_buffer;
    Renderbuffer    m_depth_buffer;
    ShaderProgram&  m_program;
    unsigned        m_nr_samples;
};

} // ns glare
