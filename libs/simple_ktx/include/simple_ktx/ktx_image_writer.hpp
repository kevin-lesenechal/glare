#pragma once

#include "simple_ktx.hpp"
#include "ktx_image_base_iterator.hpp"

namespace simple_ktx {

class KtxImageWriteIterator : public KtxImageBaseIterator
{
public:
    KtxImageWriteIterator(const KtxFile& file, std::ostream& output);

    KtxImage& operator*() noexcept { return m_image; }
    KtxImage* operator->() noexcept { return &m_image; }

    KtxImageWriteIterator& operator++();

private:
    void write_current();

private:
    const KtxFile&  m_file;
    std::ostream&   m_output;
};

class KtxImageWriter
{
public:
    KtxImageWriter(const KtxFile& file, std::ostream& output);

    KtxImageWriteIterator begin();
    nullptr_t end();

private:
    const KtxFile& m_file;
    std::ostream& m_output;
};

} // ns simple_ktx
