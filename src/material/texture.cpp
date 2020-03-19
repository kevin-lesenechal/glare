#include "glare/material/texture.hpp"

namespace glare {

Texture::Texture(Type type, int unit)
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

void Texture::set_image(GLenum target, int level, unsigned width, unsigned height,
                        GLenum internal_format, Texture::PixelFormat px_format,
                        GLenum px_type, uint8_t* data)
{
    bind();
    glTexImage2D(
        target, level, internal_format,
        width, height, 0, static_cast<GLenum>(px_format),
        px_type, data
    );
}

void Texture::set_image(int level, unsigned width, unsigned height,
                        GLenum internal_format, Texture::PixelFormat px_format,
                        GLenum px_type, uint8_t* data)
{
    set_image(static_cast<GLenum>(m_type), level, width, height,
              internal_format, px_format, px_type, data);
}

void Texture::set_compressed_image(GLenum target, int level,
                                   unsigned width, unsigned height,
                                   GLenum internal_format,
                                   uint8_t* data, size_t size)
{
    bind();
    glCompressedTexImage2D(
        target, level, internal_format, width, height, 0, size, data
    );
}

void Texture::set_compressed_image(int level, unsigned width, unsigned height,
                                   GLenum internal_format, uint8_t* data,
                                   size_t size)
{
    set_compressed_image(static_cast<GLenum>(m_type), level, width, height,
                         internal_format, data, size);
}

void Texture::generate_mipmap()
{
    bind();
    glGenerateMipmap(static_cast<GLenum>(m_type));
}

void Texture::set_parameter(GLenum param, int value)
{
    bind();
    glTexParameteri(static_cast<GLenum>(m_type), param, value);
}

} // ns glare