/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include <glare/opengl/extensions.hpp>
#include <stdexcept>
#include "glare/material/texture.hpp"

namespace glare {

Texture::Texture(Type type, int unit)
  : m_unit(unit),
    m_type(type),
    m_immutable(false)
{
    if (ext::has_dsa) {
        glCreateTextures(static_cast<GLenum>(type), 1, &m_id);
    } else {
        glGenTextures(1, &m_id);
    }
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

void Texture::set_image(GLenum target,
                        int mipmap_level,
                        AnySize image_size,
                        GLenum internal_format,
                        GLenum data_format,
                        GLenum data_type,
                        const uint8_t* data)
{
    if (m_immutable) {
        throw std::logic_error("Cannot set image on an immutable texture");
    }

    bind();

    switch (image_size.tag) {
    case AnySize::Tag::Size1D:
        glTexImage1D(
            target,
            mipmap_level,
            internal_format,
            image_size.size_1d.width,
            0,
            data_format,
            data_type,
            data
        );
        break;

    case AnySize::Tag::Size2D:
        glTexImage2D(
            target,
            mipmap_level,
            internal_format,
            image_size.size_2d.width, image_size.size_2d.height,
            0,
            data_format,
            data_type,
            data
        );
        break;

    case AnySize::Tag::Size3D:
        glTexImage3D(
            target,
            mipmap_level,
            internal_format,
            image_size.size_3d.width,
            image_size.size_3d.height,
            image_size.size_3d.depth,
            0,
            data_format,
            data_type,
            data
        );
        break;

    default:
        std::abort();
    }
}

void Texture::set_image(int mipmap_level,
                        AnySize image_size,
                        GLenum internal_format,
                        GLenum data_format,
                        GLenum data_type,
                        const uint8_t* data)
{
    set_image(
        static_cast<GLenum>(m_type),
        mipmap_level,
        image_size,
        internal_format,
        data_format,
        data_type,
        data
    );
}

void Texture::set_compressed_image(GLenum target,
                                   int mipmap_level,
                                   AnySize image_size,
                                   GLenum internal_format,
                                   const uint8_t* data,
                                   size_t size)
{
    if (m_immutable) {
        throw std::logic_error(
            "Cannot set compressed image on an immutable texture"
        );
    }

    bind();

    switch (image_size.tag) {
    case AnySize::Tag::Size1D:
        glCompressedTexImage1D(
            target,
            mipmap_level,
            internal_format,
            image_size.size_1d.width,
            0,
            size,
            data
        );
        break;

    case AnySize::Tag::Size2D:
        glCompressedTexImage2D(
            target,
            mipmap_level,
            internal_format,
            image_size.size_2d.width, image_size.size_2d.height,
            0,
            size,
            data
        );
        break;

    case AnySize::Tag::Size3D:
        glCompressedTexImage3D(
            target,
            mipmap_level,
            internal_format,
            image_size.size_3d.width,
            image_size.size_3d.height,
            image_size.size_3d.depth,
            0,
            size,
            data
        );
        break;

    default:
        std::abort();
    }
}

void Texture::set_compressed_image(int mipmap_level,
                                   AnySize image_size,
                                   GLenum internal_format,
                                   const uint8_t* data,
                                   size_t size)
{
    set_compressed_image(
        static_cast<GLenum>(m_type),
        mipmap_level,
        image_size,
        internal_format,
        data,
        size
    );
}

void Texture::set_subimage(int mipmap_level,
                           AnySize image_size,
                           GLenum data_format,
                           GLenum data_type,
                           const uint8_t* data,
                           AnyPoint offset)
{
    if (ext::has_dsa) {
        switch (image_size.tag) {
        case AnySize::Tag::Size1D:
            glTextureSubImage1D(
                m_id,
                mipmap_level,
                offset.point_1d.x,
                image_size.size_1d.width,
                data_format,
                data_type,
                data
            );
            break;

        case AnySize::Tag::Size2D:
            glTextureSubImage2D(
                m_id,
                mipmap_level,
                offset.point_2d.x, offset.point_2d.y,
                image_size.size_2d.width, image_size.size_2d.height,
                data_format,
                data_type,
                data
            );
            break;

        case AnySize::Tag::Size3D:
            glTextureSubImage3D(
                m_id,
                mipmap_level,
                offset.point_3d.x, offset.point_3d.y, offset.point_3d.z,
                image_size.size_3d.width,
                image_size.size_3d.height,
                image_size.size_3d.depth,
                data_format,
                data_type,
                data
            );
            break;

        default:
            std::abort();
        }

    } else {
        bind();
        switch (image_size.tag) {
        case AnySize::Tag::Size1D:
            glTexSubImage1D(
                static_cast<GLenum>(m_type),
                mipmap_level,
                offset.point_1d.x,
                image_size.size_1d.width,
                data_format,
                data_type,
                data
            );
            break;

        case AnySize::Tag::Size2D:
            glTexSubImage2D(
                static_cast<GLenum>(m_type),
                mipmap_level,
                offset.point_2d.x, offset.point_2d.y,
                image_size.size_2d.width, image_size.size_2d.height,
                data_format,
                data_type,
                data
            );
            break;

        case AnySize::Tag::Size3D:
            glTexSubImage3D(
                static_cast<GLenum>(m_type),
                mipmap_level,
                offset.point_3d.x, offset.point_3d.y, offset.point_3d.z,
                image_size.size_3d.width,
                image_size.size_3d.height,
                image_size.size_3d.depth,
                data_format,
                data_type,
                data
            );
            break;

        default:
            std::abort();
        }
    }
}

void Texture::set_compressed_subimage(int mipmap_level,
                                      AnySize image_size,
                                      GLenum data_format,
                                      size_t data_size,
                                      const uint8_t* data,
                                      AnyPoint offset)
{
    if (ext::has_dsa) {
        switch (image_size.tag) {
        case AnySize::Tag::Size1D:
            glCompressedTextureSubImage1D(
                m_id,
                mipmap_level,
                offset.point_1d.x,
                image_size.size_1d.width,
                data_format,
                data_size,
                data
            );
            break;

        case AnySize::Tag::Size2D:
            glCompressedTextureSubImage2D(
                m_id,
                mipmap_level,
                offset.point_2d.x, offset.point_2d.y,
                image_size.size_2d.width,
                image_size.size_2d.height,
                data_format,
                data_size,
                data
            );
            break;

        case AnySize::Tag::Size3D:
            glCompressedTextureSubImage3D(
                m_id,
                mipmap_level,
                offset.point_3d.x, offset.point_3d.y, offset.point_3d.z,
                image_size.size_3d.width,
                image_size.size_3d.height,
                image_size.size_3d.depth,
                data_format,
                data_size,
                data
            );
            break;

        default:
            std::abort();
        }

    } else {
        bind();
        switch (image_size.tag) {
        case AnySize::Tag::Size1D:
            glCompressedTexSubImage1D(
                static_cast<GLenum>(m_type),
                mipmap_level,
                offset.point_1d.x,
                image_size.size_1d.width,
                data_format,
                data_size,
                data
            );
            break;

        case AnySize::Tag::Size2D:
            glCompressedTexSubImage2D(
                static_cast<GLenum>(m_type),
                mipmap_level,
                offset.point_2d.x, offset.point_2d.y,
                image_size.size_2d.width,
                image_size.size_2d.height,
                data_format,
                data_size,
                data
            );
            break;

        case AnySize::Tag::Size3D:
            glCompressedTexSubImage3D(
                static_cast<GLenum>(m_type),
                mipmap_level,
                offset.point_3d.x, offset.point_3d.y, offset.point_3d.z,
                image_size.size_3d.width,
                image_size.size_3d.height,
                image_size.size_3d.depth,
                data_format,
                data_size,
                data
            );
            break;

        default:
            std::abort();
        }
    }
}

void Texture::allocate_storage(GLint internal_format,
                               unsigned mipmap_levels,
                               AnySize size)
{
    if (!ext::has_tex_storage) {
        throw std::runtime_error("Texture storage is not supported");
    }

    if (ext::has_dsa) {
        switch (size.tag) {
        case AnySize::Tag::Size1D:
            glTextureStorage1D(
                m_id,
                mipmap_levels,
                internal_format,
                size.size_1d.width
            );
            break;

        case AnySize::Tag::Size2D:
            glTextureStorage2D(
                m_id,
                mipmap_levels,
                internal_format,
                size.size_2d.width, size.size_2d.height
            );
            break;

        case AnySize::Tag::Size3D:
            glTextureStorage3D(
                m_id,
                mipmap_levels,
                internal_format,
                size.size_3d.width, size.size_3d.height, size.size_3d.depth
            );
            break;

        default:
            std::abort();
        }
    } else {
        bind();
        switch (size.tag) {
        case AnySize::Tag::Size1D:
            glTexStorage1D(
                static_cast<GLenum>(m_type),
                mipmap_levels,
                internal_format,
                size.size_1d.width
            );
            break;

        case AnySize::Tag::Size2D:
            glTexStorage2D(
                static_cast<GLenum>(m_type),
                mipmap_levels,
                internal_format,
                size.size_2d.width, size.size_2d.height
            );
            break;

        case AnySize::Tag::Size3D:
            glTexStorage3D(
                static_cast<GLenum>(m_type),
                mipmap_levels,
                internal_format,
                size.size_3d.width, size.size_3d.height, size.size_3d.depth
            );
            break;

        default:
            std::abort();
        }
    }

    m_immutable = true;
}

void Texture::generate_mipmap()
{
    if (ext::has_dsa) {
        glGenerateTextureMipmap(m_id);
    } else {
        bind();
        glGenerateMipmap(static_cast<GLenum>(m_type));
    }
}

void Texture::set_parameter(GLenum param, int value)
{
    if (ext::has_dsa) {
        glTextureParameteri(m_id, param, value);
    } else {
        bind();
        glTexParameteri(static_cast<GLenum>(m_type), param, value);
    }
}

void Texture::set_parameter(GLenum param, float value)
{
    if (ext::has_dsa) {
        glTextureParameterf(m_id, param, value);
    } else {
        bind();
        glTexParameterf(static_cast<GLenum>(m_type), param, value);
    }
}

AnySize Texture::size_param_for(Texture::Type type,
                                unsigned width,
                                unsigned height,
                                unsigned depth,
                                unsigned array_layer,
                                unsigned faces)
{
    switch (type) {
    case Texture::Type::Texture1D:
        return Size1D(width);

    case Texture::Type::Texture2D:
    case Texture::Type::Rectangle:
    case Texture::Type::CubeMap:
        return Size2D(width, height);

    case Texture::Type::Texture1DArray:
        return Size2D(width, array_layer);

    case Texture::Type::Texture3D:
        return Size3D(width, height, depth);

    case Texture::Type::Texture2DArray:
        return Size3D(width, height, array_layer);

    case Texture::Type::CubeMapArray:
        return Size3D(width, height, faces);

    default:
        std::abort();
    }
}

AnySize Texture::index_param_for(Texture::Type type,
                                 unsigned width,
                                 unsigned height,
                                 unsigned depth,
                                 unsigned array_layer,
                                 unsigned face_index,
                                 bool for_tex_storage)
{
    switch (type) {
    case Texture::Type::Texture1D:
        return Size1D(width);

    case Texture::Type::Texture2D:
    case Texture::Type::Rectangle:
        return Size2D(width, height);

    case Texture::Type::CubeMap:
        return for_tex_storage ? AnySize(Size3D(width, height, 1))
                               : AnySize(Size2D(width, height));

    case Texture::Type::Texture1DArray:
        return Size2D(width, array_layer);

    case Texture::Type::Texture3D:
        return Size3D(width, height, depth);

    case Texture::Type::Texture2DArray:
        return Size3D(width, height, array_layer);

    case Texture::Type::CubeMapArray:
        return Size3D(width, height, face_index);

    default:
        std::abort();
    }
}

} // ns glare
