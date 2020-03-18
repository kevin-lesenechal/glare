#pragma once

#include <glare/shader/shader.hpp>
#include <glare/shader/shader_program.hpp>
#include <glare/opengl/buffer.hpp>
#include <glare/opengl/vertex_array.hpp>

namespace glare {

struct RenderContext // FIXME: update that
{
    const glm::mat4& model_mx;
    const glm::mat4& view_mx;
    const glm::mat4& proj_mx;
};

class AxisPrimitive
{
public:
    AxisPrimitive();

    void draw(const RenderContext& ctx);

private:
    VertexBuffer<float> m_vertices;
    VertexArray         m_vao;
    Shader              m_vert_shader;
    Shader              m_frag_shader;
    ShaderProgram       m_program;
};

} // ns glare
