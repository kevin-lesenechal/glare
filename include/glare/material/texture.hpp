/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <epoxy/gl.h>

#include "glare/sizes.hpp"
#include "glare/points.hpp"

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

public:
    explicit Texture(Type type);

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    ~Texture() noexcept;

    void bind();
    void bind(unsigned unit);

    void set_image(GLenum target,
                   int mipmap_level,
                   AnySize image_size,
                   GLenum internal_format,
                   GLenum data_format,
                   GLenum data_type,
                   const uint8_t* data);

    void set_image(int mipmap_level,
                   AnySize image_size,
                   GLenum internal_format,
                   GLenum data_format,
                   GLenum data_type,
                   const uint8_t* data);

    void set_compressed_image(GLenum target,
                              int mipmap_level,
                              AnySize image_size,
                              GLenum internal_format,
                              const uint8_t* data,
                              size_t size);

    void set_compressed_image(int mipmap_level,
                              AnySize image_size,
                              GLenum internal_format,
                              const uint8_t* data,
                              size_t size);

    void set_subimage(int mipmap_level,
                      AnySize image_size,
                      GLenum data_format,
                      GLenum data_type,
                      const uint8_t* data,
                      AnyPoint offset = Point3D(0, 0, 0));

    void set_compressed_subimage(int mipmap_level,
                                 AnySize image_size,
                                 GLenum data_format,
                                 size_t data_size,
                                 const uint8_t* data,
                                 AnyPoint offset = Point3D(0, 0, 0));

    void allocate_storage(GLint internal_format,
                          unsigned mipmap_levels,
                          AnySize size);

    void allocate_multisample_storage(GLenum internal_format,
                                      unsigned samples,
                                      AnySize image_size,
                                      bool fixed_locations);

    void generate_mipmap();

    void set_mipmap_range(unsigned min_level, unsigned max_level);
    void set_mipmap_count(unsigned nr_levels);

    void set_parameter(GLenum param, int value);

    void set_parameter(GLenum param, float value);

    [[nodiscard]]
    GLuint id() const noexcept { return m_id; }

    [[nodiscard]]
    Type type() const noexcept { return m_type; }

public:
    static AnySize size_param_for(Type type,
                                  unsigned width,
                                  unsigned height,
                                  unsigned depth,
                                  unsigned array_layer,
                                  unsigned faces);

    static AnySize index_param_for(Type type,
                                   unsigned width,
                                   unsigned height,
                                   unsigned depth,
                                   unsigned array_layer,
                                   unsigned face_index,
                                   bool for_tex_storage = false);

private:
    Type   m_type;
    GLuint m_id;
    bool   m_immutable;
};

} // ns glare
