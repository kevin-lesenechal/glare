/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/primitive/axis.hpp"

namespace glare {

AxisPrimitive::AxisPrimitive()
  : m_vert_shader(Shader::from_source_file(
        Shader::Type::Vertex,
        "shaders/solid.vert.glsl"
    )),
    m_frag_shader(Shader::from_source_file(
        Shader::Type::Fragment,
        "shaders/solid.frag.glsl"
    )),
    m_program({&m_vert_shader, &m_frag_shader})
{
    std::vector<float> vert_data = {
        0.0f, 0.0f, 0.0f,     1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,     1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,     0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,     0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,     0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,     0.0f, 0.0f, 1.0f
    };

    for (int x = -20; x <= 20; ++x) {
        vert_data.insert(
            vert_data.end(),
            {static_cast<float>(x), -20.0f, 0.0f,   0.5f, 0.5f, 0.5f,
             static_cast<float>(x),  20.0f, 0.0f,   0.5f, 0.5f, 0.5f});
    }

    for (int y = -20; y <= 20; ++y) {
        vert_data.insert(
            vert_data.end(),
            {-20.0f, static_cast<float>(y), 0.0f,   0.5f, 0.5f, 0.5f,
              20.0f, static_cast<float>(y), 0.0f,   0.5f, 0.5f, 0.5f});
    }

    m_vertices.upload(BufferUsage::StaticDraw, vert_data);

    m_program.link();
    m_vao.attach(0, m_vertices, 3, 24, 0);
    m_vao.attach(1, m_vertices, 3, 24, 12);
}

void AxisPrimitive::draw(const RenderContext& ctx)
{
    m_program.use();
    m_vao.bind();

    m_program.set_uniform("g_model_mx", ctx.model_mx);
    m_program.set_uniform("g_view_mx", ctx.view_mx);
    m_program.set_uniform("g_proj_mx", ctx.proj_mx);

    glLineWidth(1.0f);
    glDrawArrays(GL_LINES, 6, 164);
    glLineWidth(5.0f);
    glDrawArrays(GL_LINES, 0, 6);
    glLineWidth(1.0f);
}

} // ns glare
