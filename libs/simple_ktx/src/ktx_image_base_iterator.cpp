/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the libsimple_ktx project, published under the terms  *
 * of the MIT license; see LICENSE file for more information.                 *
 ******************************************************************************/

#include "simple_ktx/ktx_image_base_iterator.hpp"

namespace simple_ktx {

KtxImageBaseIterator::KtxImageBaseIterator(const KtxFileHeader& header)
  : m_header(header),
    m_has_next(true)
{}

KtxImageBaseIterator& KtxImageBaseIterator::operator++()
{
    if (!m_has_next) {
        return *this;
    }

    if (++m_image.z_slice >= m_header.pixel_depth) {
        m_image.z_slice = 0;

        if (++m_image.face_index >= m_header.nr_faces) {
            m_image.face_index = 0;

            if (++m_image.array_index >= m_header.nr_array_elements) {
                m_image.array_index = 0;

                if (++m_image.mipmap_level >= m_header.nr_mipmap_levels) {
                    m_has_next = false;
                }
            }
        }
    }

    return *this;
}

void KtxImageBaseIterator::update_dimensions(
    const gl_tables::FormatBlockSize& block_format)
{
    m_image.px_width = m_header.pixel_width >> m_image.mipmap_level;
    m_image.px_height = m_header.pixel_height >> m_image.mipmap_level;

    m_image.x_blocks = m_image.px_width / block_format.width;
    if (m_image.x_blocks == 0) {
        m_image.x_blocks = 1;
    }

    m_image.y_blocks = m_image.px_height / block_format.height;
    if (m_image.y_blocks == 0) {
        m_image.y_blocks = 1;
    }
}

} // ns simple_ktx
