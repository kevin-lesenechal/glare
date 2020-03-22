/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the libsimple_ktx project, published under the terms  *
 * of the MIT license; see LICENSE file for more information.                 *
 ******************************************************************************/

#pragma once

#include "simple_ktx/simple_ktx.hpp"

#include <gl_tables.hpp>

namespace simple_ktx {

struct KtxFileHeader;

class KtxImageBaseIterator
{
public:
    explicit KtxImageBaseIterator(const KtxFileHeader& header);

    bool operator==(nullptr_t) const noexcept { return !m_has_next; }
    bool operator!=(nullptr_t) const noexcept { return m_has_next; }

protected:
    KtxImageBaseIterator& operator++();
    void update_dimensions(const gl_tables::FormatBlockSize& block_format);

protected:
    const KtxFileHeader& m_header;
    KtxImage             m_image;
    bool                 m_has_next;
};

} // ns simple_ktx
