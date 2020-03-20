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
