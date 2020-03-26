/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the libsimple_ktx project, published under the terms  *
 * of the MIT license; see LICENSE file for more information.                 *
 ******************************************************************************/

#pragma once

#include <gl_tables.hpp>

#include <cstdint>
#include <istream>
#include <map>
#include <vector>
#include <cstring>

namespace simple_ktx {

inline constexpr char KTX_MAGIC[] = {
    '\xab', 'K', 'T', 'X', ' ', '1', '1', '\xbb', '\r', '\n', '\x1a', '\n'
};

struct KtxFileHeader
{
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

    KtxFileHeader()
      : endianness(0x04030201),
        gl_type(0),
        gl_type_size(0),
        gl_format(0),
        gl_internal_format(0),
        gl_base_internal_format(0),
        pixel_width(0),
        pixel_height(0),
        pixel_depth(0),
        nr_array_elements(0),
        nr_faces(1),
        nr_mipmap_levels(0),
        key_value_bytes(0)
    {
        std::memcpy(magic, KTX_MAGIC, sizeof magic);
    }
} __attribute__((packed));

struct KtxImage
{
    unsigned mipmap_level = 0;
    unsigned array_index = 0;
    unsigned face_index = 0;
    unsigned z_slice = 0;

    unsigned px_width;
    unsigned px_height;
    unsigned x_blocks;
    unsigned y_blocks;

    std::vector<uint8_t> data;
};

class KtxImageReader;
class KtxImageWriter;

class KtxFile
{
public:
    KtxFileHeader header;
    std::map<std::string, std::string> key_values;
    gl_tables::FormatBlockSize block_format;

    KtxImageReader read_images_from(std::istream& input) const;
    KtxImageWriter write_images_to(std::ostream& output);
    void rewind_after_header(std::istream& input) const;
};

KtxFile read_ktx_header(std::istream& input);
void write_ktx_header(std::ostream& output, KtxFile& file);

void print_header(FILE* fh, const KtxFileHeader& header);

} // ns simple_ktx
