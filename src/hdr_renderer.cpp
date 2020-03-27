/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/hdr_renderer.hpp"
#include "glare/shader/shader_program.hpp"

namespace glare {

HdrRenderer::HdrRenderer(ShaderProgram& program)
  : m_framebuffer(Framebuffer::Target::ReadDraw),
    m_color_buffer(Texture::Type::Texture2D),
    m_program(program)
{
    m_color_buffer.set_image(
        0, Size2D(1280, 1024), GL_RGB16F, GL_RGBA, GL_FLOAT, nullptr
    );
    m_color_buffer.set_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_color_buffer.set_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_depth_buffer.allocate_storage(GL_DEPTH_COMPONENT, Size2D(1280, 1024));

    m_framebuffer.attach(m_color_buffer, Framebuffer::Attachment::Color0);
    m_framebuffer.attach(m_depth_buffer, Framebuffer::Attachment::Depth);
    m_framebuffer.ensure_complete();
    m_framebuffer.unbind();
}

void HdrRenderer::start_scene()
{
    m_framebuffer.bind();
}

void HdrRenderer::end_scene_draw()
{
    m_framebuffer.unbind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program.use();
    m_color_buffer.bind();
    m_program.set_uniform("u_color", m_color_buffer.unit());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

} // ns glare
