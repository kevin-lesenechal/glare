#pragma once

#include <epoxy/gl.h>
#include <cstdint>
#include <istream>

namespace glare {

class Texture;

struct KtxFileHeader
{
    static constexpr char MAGIC[] = {
        '\xab', 'K', 'T', 'X', ' ', '1', '1', '\xbb', '\r', '\n', '\x1a', '\n'
    };

    uint8_t  magic[12];
    uint32_t endianness;
    uint32_t gl_type;
    uint32_t gl_type_size;
    uint32_t gl_format;
    uint32_t gl_internal_format;
    uint32_t gl_base_internal_format;
    uint32_t pixel_width;
    uint32_t pixel_height;
    uint32_t pixel_depth;
    uint32_t nr_array_elements;
    uint32_t nr_faces;
    uint32_t nr_mipmap_levels;
    uint32_t key_value_bytes;
} __attribute__((packed));

class KtxFileLoader
{
public:
    KtxFileLoader();

    Texture load_texture(std::istream& input, int unit = 0);

private:
    void load_mipmap_levels(const KtxFileHeader& header,
                            std::istream& input,
                            Texture& texture);

    void load_image(const KtxFileHeader& header,
                    std::istream& input,
                    Texture& texture,
                    unsigned level,
                    GLenum tex_target);
};

} // ns glare
