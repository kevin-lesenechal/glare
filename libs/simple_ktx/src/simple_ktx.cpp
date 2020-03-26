/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the libsimple_ktx project, published under the terms  *
 * of the MIT license; see LICENSE file for more information.                 *
 ******************************************************************************/

#include "simple_ktx/simple_ktx.hpp"
#include "simple_ktx/ktx_image_reader.hpp"
#include "simple_ktx/ktx_image_writer.hpp"

#include <cstring>
#include <memory>
#include <iostream>

namespace simple_ktx {

KtxFile read_ktx_header(std::istream& input)
{
    KtxFile file;
    KtxFileHeader& header = file.header;

    auto except = input.exceptions();
    input.exceptions(std::ios::failbit);
    input.read(reinterpret_cast<char*>(&header), sizeof header);

    if (std::memcmp(header.magic, KTX_MAGIC, sizeof header.magic) != 0) {
        throw std::runtime_error("KTX: invalid magic signature in header");
    }

    if (header.endianness != 0x04030201) {
        throw std::runtime_error("KTX: invalid endianness");
    }

    if (header.key_value_bytes > 0) {
        auto key_values = std::unique_ptr<uint8_t[]>(
            new uint8_t[header.key_value_bytes]
        );
        input.read(
            reinterpret_cast<char*>(key_values.get()),
            header.key_value_bytes
        );

        for (unsigned off = 0; off < header.key_value_bytes;) {
            uint32_t kv_size;
            if (header.key_value_bytes < off + 6) {
                throw std::runtime_error("Unexpected end of key values");
            }
            std::memcpy(&kv_size, key_values.get() + off, sizeof kv_size);
            off += 4;

            uint8_t* end = reinterpret_cast<uint8_t*>(
                std::memchr(key_values.get() + off, '\0',
                            header.key_value_bytes - off)
            );
            if (end == nullptr) {
                throw std::runtime_error("Key: expecting '\\0'");
            }

            size_t key_len = end - (key_values.get() + off);
            std::string key(
                reinterpret_cast<char*>(key_values.get() + off), key_len
            );
            off += key_len + 1;

            end = reinterpret_cast<uint8_t*>(
                std::memchr(key_values.get() + off, '\0',
                            header.key_value_bytes - off)
            );
            if (end == nullptr) {
                throw std::runtime_error("Value: expecting '\\0'");
            }

            size_t val_len = end - (key_values.get() + off);
            std::string value(
                reinterpret_cast<char*>(key_values.get() + off), val_len
            );
            off += val_len + 1;
            off = (off + 3) & ~3;

            file.key_values[key] = value;
        }
    }

    input.exceptions(except);

    file.block_format = gl_tables::format_block_size(header.gl_internal_format);

    return file;
}

void write_ktx_header(std::ostream& output, KtxFile& file)
{
    uint32_t kv_total_size = 0;

    for (const auto& kv : file.key_values) {
        uint32_t kv_size = 4 + kv.first.size() + 1 + kv.second.size() + 1;
        kv_size = (kv_size + 3) & ~3;
        kv_total_size += kv_size;
    }

    file.header.key_value_bytes = kv_total_size;
    file.block_format = gl_tables::format_block_size(
        file.header.gl_internal_format
    );

    auto except = output.exceptions();
    output.exceptions(std::ios::failbit);
    output.write(
        reinterpret_cast<const char*>(&file.header),
        sizeof file.header
    );

    for (const auto& kv : file.key_values) {
        uint32_t kv_size = 4 + kv.first.size() + 1 + kv.second.size() + 1;
        output.write(reinterpret_cast<const char*>(&kv_size), sizeof kv_size);
        output.write(kv.first.c_str(), kv.first.size() + 1);
        output.write(kv.second.c_str(), kv.second.size() + 1);

        size_t padlen = 3 - ((kv_size + 3) % 4);
        for (size_t i = 0; i < padlen; ++i) {
            output.put('\0');
        }
    }

    output.exceptions(except);
}

void print_header(FILE* fh, const KtxFileHeader& h)
{
    std::string type = gl_tables::type_name(h.gl_type);
    std::string format = gl_tables::format_name(h.gl_format);
    std::string in_format = gl_tables::internal_format_name(h.gl_internal_format);

    fprintf(fh, "    Dimensions       %u × %u × %u\n",
            h.pixel_width, h.pixel_height, h.pixel_depth);
    fprintf(fh, "    Type             0x%.4x  %s  [size = %u]\n",
            h.gl_type, type.c_str(), h.gl_type_size);
    fprintf(fh, "    Format           0x%.4x  %s\n",
            h.gl_format, format.c_str());
    fprintf(fh, "    Internal format  0x%.4x  %s\n",
            h.gl_internal_format, in_format.c_str());
    fprintf(fh, "    # array elems.   %u\n",
            h.nr_array_elements);
    fprintf(fh, "    # faces          %u\n",
            h.nr_faces);
    fprintf(fh, "    # mipmap levels  %u\n",
            h.nr_mipmap_levels);
}

KtxImageReader KtxFile::read_images_from(std::istream& input) const
{
    return KtxImageReader(*this, input);
}

KtxImageWriter KtxFile::write_images_to(std::ostream& output)
{
    return KtxImageWriter(*this, output);
}

void KtxFile::rewind_after_header(std::istream& input) const
{
    size_t header_size = sizeof header + header.key_value_bytes;
    input.seekg(header_size, std::ios::beg);
}

} // ns simple_ktx
