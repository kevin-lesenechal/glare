#include "glare/material/ktx_loader.hpp"
#include "glare/material/texture.hpp"

#include <gl_tables.hpp>

#include <cstring>
#include <memory>

namespace glare {

KtxFileLoader::KtxFileLoader()
{}

Texture KtxFileLoader::load_texture(std::istream& input, int unit)
{
    KtxFileHeader header;

    input.read(reinterpret_cast<char*>(&header), sizeof header);

    if (std::memcmp(header.magic, KtxFileHeader::MAGIC, sizeof header.magic) != 0) {
        throw std::runtime_error("KTX: invalid magic signature in header");
    }

    if (header.nr_array_elements == 0 && header.nr_faces == 1) {
        Texture texture(Texture::Type::Texture2D, unit);
        load_mipmap_levels(header, input, texture);
        return texture;
    } else if (header.nr_array_elements == 0 && header.nr_faces == 6) {
        Texture texture(Texture::Type::CubeMap, unit);
        load_mipmap_levels(header, input, texture);
        return texture;
    } else {
        throw std::runtime_error("KTX: texture type not supported");
    }
}

void KtxFileLoader::load_mipmap_levels(const KtxFileHeader& header,
                                       std::istream& input,
                                       Texture& texture)
{
    unsigned level_count;
    if (header.nr_mipmap_levels == 0) {
        level_count = 1;
    } else {
        level_count = header.nr_mipmap_levels;
    }

    for (unsigned level = 0; level < level_count; ++level) {
        uint32_t total_level_size;
        input.read(
            reinterpret_cast<char*>(&total_level_size),
            sizeof total_level_size
        );

        switch (texture.type()) {
            case Texture::Type::Texture2D:
                load_image(header, input, texture, level, GL_TEXTURE_2D);
                break;

            case Texture::Type::CubeMap:
                for (unsigned i = 0; i < header.nr_faces; ++i) {
                    load_image(header, input, texture, level,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
                }
                break;

            default:
                std::abort();
        }
    }

    if (header.nr_mipmap_levels == 0) {
        texture.generate_mipmap();
    } else  if (header.nr_mipmap_levels > 1) {
        texture.set_parameter(GL_TEXTURE_MAX_LEVEL, header.nr_mipmap_levels);
    }
}

void KtxFileLoader::load_image(const KtxFileHeader& header,
                               std::istream& input,
                               Texture& texture,
                               unsigned level,
                               GLenum tex_target)
{
    unsigned width = header.pixel_width >> level;
    unsigned height = header.pixel_height >> level;
    auto block_size = gl_tables::format_block_size(header.gl_internal_format);

    // FIXME: height might be = 0 (1D)
    size_t image_size = width * height
                        * (block_size.size_bits / 8)
                        / (block_size.width * block_size.height);
    image_size = std::max(image_size, static_cast<size_t>(block_size.size_bits / 8));

    auto image_data = std::unique_ptr<uint8_t[]>(new uint8_t[image_size]);
    input.read(reinterpret_cast<char*>(image_data.get()), image_size);

    if (header.gl_type == 0) { // Compressed
        texture.set_compressed_image(
            tex_target,
            static_cast<int>(level),
            width, height,
            header.gl_internal_format,
            image_data.get(), image_size
        );
    } else {
        texture.set_image(
            tex_target,
            static_cast<int>(level),
            width, height,
            header.gl_internal_format,
            static_cast<Texture::PixelFormat>(header.gl_format),
            header.gl_type,
            image_data.get()
        );
    }
}

} // ns glare
