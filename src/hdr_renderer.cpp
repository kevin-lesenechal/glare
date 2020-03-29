/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/hdr_renderer.hpp"
#include "glare/shader/shader_program.hpp"

namespace glare {

HdrRenderer::HdrRenderer(ShaderProgram& program,
                         Size2D size,
                         unsigned nr_samples)
  : m_framebuffer(Framebuffer::Target::ReadDraw),
    m_color_buffer(nr_samples > 0 ? Texture::Type::Texture2DMultisample
                                  : Texture::Type::Texture2D),
    m_program(program),
    m_nr_samples(nr_samples)
{
    setup_buffers(m_color_buffer, m_depth_buffer, size);

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
    m_color_buffer.bind(0);

    if (m_nr_samples > 0) {
        m_program.set_uniform("u_color_ms", 0);
    } else {
        m_program.set_uniform("u_color", 0);
    }
    m_program.set_uniform("u_nr_samples", m_nr_samples);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void HdrRenderer::setup_buffers(Texture& color_buffer,
                                Renderbuffer& depth_buffer,
                                Size2D size)
{
    if (m_nr_samples > 0) {
        color_buffer.allocate_multisample_storage(
            GL_RGB16F, m_nr_samples, size, true
        );
    } else {
        color_buffer.set_image(
            0, size, GL_RGB16F, GL_RGBA, GL_FLOAT, nullptr
        );
        color_buffer.set_mipmap_count(1);
        color_buffer.set_parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        color_buffer.set_parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    depth_buffer.allocate_storage(GL_DEPTH_COMPONENT, size, m_nr_samples);
}

} // ns glare
