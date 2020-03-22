/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <epoxy/gl.h>

namespace glare {

class Texture
{
public:
    enum class Type : GLenum
    {
        Texture1D                 = GL_TEXTURE_1D,
        Texture2D                 = GL_TEXTURE_2D,
        Texture3D                 = GL_TEXTURE_3D,
        Texture1DArray            = GL_TEXTURE_1D_ARRAY,
        Texture2DArray            = GL_TEXTURE_2D_ARRAY,
        Rectangle                 = GL_TEXTURE_RECTANGLE,
        CubeMap                   = GL_TEXTURE_CUBE_MAP,
        CubeMapArray              = GL_TEXTURE_CUBE_MAP_ARRAY,
        Buffer                    = GL_TEXTURE_BUFFER,
        Texture2DMultisample      = GL_TEXTURE_2D_MULTISAMPLE,
        Texture2DMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
    };

    enum class PixelFormat : GLenum
    {
        R               = GL_RED,
        RG              = GL_RG,
        RGB             = GL_RGB,
        BGR             = GL_BGR,
        RGBA            = GL_RGBA,
        BGRA            = GL_BGRA,
        IntR            = GL_RED_INTEGER,
        IntRG           = GL_RG_INTEGER,
        IntRGB          = GL_RGB_INTEGER,
        IntBGR          = GL_BGR_INTEGER,
        IntRGBA         = GL_RGBA_INTEGER,
        IntBGRA         = GL_BGRA_INTEGER,
        StencilIndex    = GL_STENCIL_INDEX,
        DepthComponent  = GL_DEPTH_COMPONENT,
        DepthStencil    = GL_DEPTH_STENCIL,
    };

public:
    explicit Texture(Type type, int unit = 0);

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    ~Texture() noexcept;

    void bind();

    void set_image(GLenum target, int level, unsigned width, unsigned height,
                   GLenum internal_format, PixelFormat px_format,
                   GLenum px_type, uint8_t* data);

    void set_image(int level, unsigned width, unsigned height,
                   GLenum internal_format, PixelFormat px_format,
                   GLenum px_type, uint8_t* data);

    void set_compressed_image(GLenum target, int level,
                              unsigned width, unsigned height,
                              GLenum internal_format,
                              uint8_t* data, size_t size);

    void set_compressed_image(int level, unsigned width, unsigned height,
                              GLenum internal_format,
                              uint8_t* data, size_t size);

    void generate_mipmap();

    void set_parameter(GLenum param, int value);

    void set_parameter(GLenum param, float value);

    [[nodiscard]]
    GLuint id() const noexcept { return m_id; }

    [[nodiscard]]
    int unit() const noexcept { return m_unit; }

    [[nodiscard]]
    Type type() const noexcept { return m_type; }

private:
    int    m_unit;
    Type   m_type;
    GLuint m_id;
};

} // ns glare
