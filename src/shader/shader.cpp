#include "glare/shader/shader.hpp"

namespace glare {

Shader::Shader(Shader&& other) noexcept
    : m_type(other.m_type),
      m_id(other.m_id)
{
    if (&other != this) {
        other.m_id = 0;
    }
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (&other != this) {
        glDeleteShader(m_id);
        m_type = other.m_type;
        m_id = other.m_id;
        other.m_id = 0;
    }

    return *this;
}

Shader::~Shader() noexcept
{
    glDeleteShader(m_id);
}

void Shader::load_source(const std::string& glsl_src)
{
    const char* src_cstr = glsl_src.c_str();
    glShaderSource(m_id, 1, &src_cstr, nullptr);
}

void Shader::compile()
{
    glCompileShader(m_id);

    GLint status;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE) {
        char msg[512];
        glGetShaderInfoLog(m_id, sizeof msg, nullptr, msg);

        throw std::runtime_error("GLSL error: " + std::string(msg));
    }
}

Shader Shader::from_source_file(Shader::Type type,
                                const std::string& source_file)
{
    std::ifstream file(source_file);

    if (!file) {
        throw std::runtime_error("Couldn't open file '" + source_file + "'");
    }

    std::stringstream ss;
    ss << file.rdbuf();

    Shader shader(type);
    shader.load_source(ss.str());
    shader.compile();

    return shader;
}

Shader::Shader(Shader::Type type)
  : m_type(type)
{
    if ((m_id = glCreateShader(static_cast<GLenum>(m_type))) == 0) {
        throw 42; // TODO: proper OpenGL exception
    }
}

} // ns glare
