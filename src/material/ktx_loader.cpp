/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/material/ktx_loader.hpp"
#include "glare/opengl/extensions.hpp"

namespace ktx = simple_ktx;

namespace glare {

KtxFileLoader::KtxFileLoader(LoggerInterface& logger)
  : m_logger(logger)
{}

Texture KtxFileLoader::load_texture(std::istream& input, int unit)
{
    ktx::KtxFile file = ktx::read_ktx_header(input);
    ktx::KtxFileHeader& header = file.header;

    unsigned nr_mipmaps = header.nr_mipmap_levels;
    if (nr_mipmaps == 0) {
        throw std::runtime_error("Not implemented, calculate mipmaps for storage");
        nr_mipmaps = 1;
    }

    if (file.key_values["KTXorientation"] != "S=r,T=u") {
        throw std::runtime_error(
            "The GLARE's KTX file loader requires textures to have the OpenGL "
            "orientation (i.e. 0,0 on the bottom left)"
        );
    }

    Texture texture(texture_type(header), unit);
    AnySize total_size = Texture::size_param_for(
        texture.type(),
        header.pixel_width,
        header.pixel_height,
        header.pixel_depth,
        header.nr_array_elements,
        header.nr_faces
    );

    if (ext::has_tex_storage) {
        texture.allocate_storage(
            header.gl_internal_format,
            nr_mipmaps,
            total_size
        );
    }

    for (const auto& image : file.read_images_from(input)) {
        AnySize image_size = Texture::index_param_for(
            texture.type(),
            image.px_width,
            image.px_height,
            image.z_slice,
            image.array_index,
            image.face_index
        );

        AnyPoint offset = Point3D(0, 0, 0);

        GLenum tex_target = static_cast<GLenum>(texture.type());
        if (texture.type() == Texture::Type::CubeMap) {
            tex_target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + image.face_index;
            image_size.size_3d.depth = 1;
            offset.point_3d.z = image.face_index;
        }

        if (header.gl_type == 0) { // Compressed
            if (ext::has_tex_storage) {
                texture.set_compressed_subimage(
                    static_cast<int>(image.mipmap_level),
                    image_size,
                    header.gl_internal_format,
                    image.data.size(),
                    image.data.data(),
                    offset
                );
            } else {
                texture.set_compressed_image(
                    tex_target,
                    static_cast<int>(image.mipmap_level),
                    image_size,
                    header.gl_internal_format,
                    image.data.data(), image.data.size()
                );
            }
        } else {
            if (ext::has_tex_storage) {
                texture.set_subimage(
                    static_cast<int>(image.mipmap_level),
                    image_size,
                    header.gl_format,
                    header.gl_type,
                    image.data.data(),
                    offset
                );
            } else {
                texture.set_image(
                    tex_target,
                    static_cast<int>(image.mipmap_level),
                    image_size,
                    header.gl_internal_format,
                    header.gl_format,
                    header.gl_type,
                    image.data.data()
                );
            }
        }
    }

    if (header.nr_mipmap_levels == 0) {
        texture.generate_mipmap();
        m_logger.notice(
            "[KtxLoader] KTX file contained no mipmaps and were dynamically "
            "generated"
        );
    } else {
        texture.set_parameter(GL_TEXTURE_BASE_LEVEL, 0);
        texture.set_parameter(GL_TEXTURE_MAX_LEVEL,
                              static_cast<int>(header.nr_mipmap_levels));
    }

    texture.set_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    texture.set_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

Texture::Type KtxFileLoader::texture_type(
    const simple_ktx::KtxFileHeader& ktx_header)
{
    if (ktx_header.pixel_width > 0
        && ktx_header.pixel_height == 0
        && ktx_header.pixel_depth == 0) {
        if (ktx_header.nr_faces == 0 && ktx_header.nr_array_elements == 0) {
            return Texture::Type::Texture1D;
        } else if (ktx_header.nr_faces == 0) {
            return Texture::Type::Texture1DArray;
        }
    } else if (ktx_header.pixel_width > 0
               && ktx_header.pixel_height > 0
               && ktx_header.pixel_depth == 0) {
        if (ktx_header.nr_faces == 1 && ktx_header.nr_array_elements == 0) {
            return Texture::Type::Texture2D;
        } else if (ktx_header.nr_faces == 1) {
            return Texture::Type::Texture2DArray;
        } else if (ktx_header.nr_faces == 6) {
            if (ktx_header.nr_array_elements == 0) {
                return Texture::Type::CubeMap;
            } else {
                return Texture::Type::CubeMapArray;
            }
        }
    } else if (ktx_header.pixel_width > 0
               && ktx_header.pixel_height > 0
               && ktx_header.pixel_depth > 0) {
        if (ktx_header.nr_faces == 1 && ktx_header.nr_array_elements == 0) {
            return Texture::Type::Texture3D;
        }
    }

    throw std::runtime_error("Unsupported texture type");
}

} // ns glare
