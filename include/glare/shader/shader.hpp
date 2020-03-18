#pragma once

#include <epoxy/gl.h>
#include <string>
#include <fstream>
#include <sstream>

namespace glare {

class Shader
{
public:
    enum class Type : GLenum {
        Compute     = GL_COMPUTE_SHADER,
        Vertex      = GL_VERTEX_SHADER,
        TessControl = GL_TESS_CONTROL_SHADER,
        TessEval    = GL_TESS_EVALUATION_SHADER,
        Geometry    = GL_GEOMETRY_SHADER,
        Fragment    = GL_FRAGMENT_SHADER,
    };

public:
    explicit Shader(Type type);

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    ~Shader() noexcept;

    void load_source(const std::string& glsl_src);
    void compile();

    [[nodiscard]]
    GLuint id() const noexcept { return m_id; }

public:
    // TODO: move this out
    [[nodiscard]]
    static Shader from_source_file(Type type, const std::string& source_file);

private:
    Type    m_type;
    GLuint  m_id;
};

} // ns glare
