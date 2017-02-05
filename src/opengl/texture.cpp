#include "glare/opengl/texture.hpp"

namespace glare {

Texture::Texture(int unit, Texture::Type type)
  : m_unit(unit),
    m_type(type)
{
    glGenTextures(1, &m_id);
}

Texture::Texture(Texture&& other) noexcept
  : m_unit(other.m_unit),
    m_type(other.m_type),
    m_id(other.m_id)
{
    if (&other != this) {
        other.m_id = 0;
    }
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (&other != this) {
        glDeleteTextures(1, &m_id);
        m_id = other.m_id;
        other.m_id = 0;
        m_unit = other.m_unit;
        m_type = other.m_type;
    }

    return *this;
}

Texture::~Texture() noexcept
{
    glDeleteTextures(1, &m_id);
}

void Texture::bind()
{
    glActiveTexture(GL_TEXTURE0 + m_unit);
    glBindTexture(static_cast<GLenum>(m_type), m_id);
}

void Texture::set_image(int level, unsigned width, unsigned height,
                        GLenum internal_format, Texture::PixelFormat px_format,
                        GLenum px_type, uint8_t* data)
{
    bind();
    glTexImage2D(static_cast<GLenum>(m_type), level, internal_format,
                 width, height, 0, static_cast<GLenum>(px_format),
                 px_type, data);
}

void Texture::generate_mipmap()
{
    bind();
    glGenerateMipmap(static_cast<GLenum>(m_type));
}

} // ns glare
