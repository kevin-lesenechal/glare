/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the libsimple_ktx project, published under the terms  *
 * of the MIT license; see LICENSE file for more information.                 *
 ******************************************************************************/

#pragma once

#include "simple_ktx.hpp"
#include "ktx_image_base_iterator.hpp"

namespace simple_ktx {

class KtxImageReadIterator : public KtxImageBaseIterator
{
public:
    KtxImageReadIterator(const KtxFile& file, std::istream& input);

    const KtxImage& operator*() const noexcept { return m_image; }
    const KtxImage* operator->() const noexcept { return &m_image; }

    KtxImageReadIterator& operator++();

private:
    void read_next();

private:
    const KtxFile&  m_file;
    std::istream&   m_input;
};

class KtxImageReader
{
public:
    KtxImageReader(const KtxFile& file, std::istream& input);

    KtxImageReadIterator begin();
    nullptr_t end();

private:
    const KtxFile& m_file;
    std::istream& m_input;
};

} // ns simple_ktx
