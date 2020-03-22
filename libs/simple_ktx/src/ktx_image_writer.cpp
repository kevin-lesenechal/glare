/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the libsimple_ktx project, published under the terms  *
 * of the MIT license; see LICENSE file for more information.                 *
 ******************************************************************************/

#include "simple_ktx/ktx_image_writer.hpp"

namespace simple_ktx {

KtxImageWriteIterator::KtxImageWriteIterator(const KtxFile& file,
                                             std::ostream& output)
  : KtxImageBaseIterator(file.header),
    m_file(file),
    m_output(output)
{
    update_dimensions(m_file.block_format);
}

KtxImageWriteIterator& KtxImageWriteIterator::operator++()
{
    if (!m_has_next) {
        return *this;
    }

    write_current();

    KtxImageBaseIterator::operator++();
    update_dimensions(m_file.block_format);

    return *this;
}

void KtxImageWriteIterator::write_current()
{
    bool mipmap_start = m_image.z_slice == 0
                        && m_image.face_index == 0
                        && m_image.array_index == 0;
    bool is_cubemap = m_header.nr_faces == 6
                      && m_header.nr_array_elements == 0;
    uint32_t level_size = 0;
    unsigned bytes_per_block = m_file.block_format.size_bits / 8;
    uint32_t data_size = m_image.x_blocks * m_image.y_blocks * bytes_per_block;

    if (m_image.data.size() != data_size) {
        throw std::runtime_error("Invalid image data size");
    }

    auto except = m_output.exceptions();
    m_output.exceptions(std::ios::failbit);

    if (mipmap_start) {
        uint32_t nr_arr_elems = m_header.nr_array_elements;
        if (nr_arr_elems == 0) nr_arr_elems = 1;
        uint32_t nr_faces = m_header.nr_faces;
        if (nr_faces == 0) nr_faces = 1;
        uint32_t nr_z_slices = m_header.pixel_depth;
        if (nr_z_slices == 0) nr_z_slices = 1;

        level_size = data_size * nr_arr_elems * nr_faces * nr_z_slices;

        if (is_cubemap) {
            m_output.write(reinterpret_cast<char*>(&data_size),
                           sizeof data_size);
        } else {
            m_output.write(reinterpret_cast<char*>(&level_size),
                           sizeof level_size);
        }
    }

    m_output.write(reinterpret_cast<char*>(m_image.data.data()),
                   m_image.data.size());

    if (mipmap_start) {
        size_t padlen = 3 - ((level_size + 3) % 4);
        for (size_t i = 0; i < padlen; ++i) {
            m_output.put('\0');
        }
    }

    m_output.exceptions(except);
}

KtxImageWriter::KtxImageWriter(const KtxFile& file, std::ostream& output)
  : m_file(file),
    m_output(output)
{}

KtxImageWriteIterator KtxImageWriter::begin()
{
    return KtxImageWriteIterator(m_file, m_output);
}

nullptr_t KtxImageWriter::end()
{
    return nullptr;
}

} // ns simple_ktx
