#include "simple_ktx/ktx_image_reader.hpp"

namespace simple_ktx {

KtxImageReadIterator::KtxImageReadIterator(const KtxFile& file, std::istream& input)
  : KtxImageBaseIterator(file.header),
    m_file(file),
    m_input(input)
{
    read_next();
}

KtxImageReadIterator& KtxImageReadIterator::operator++()
{
    if (!m_has_next) {
        return *this;
    }

    KtxImageBaseIterator::operator++();

    if (m_has_next) {
        read_next();
    }

    return *this;
}

void KtxImageReadIterator::read_next()
{
    bool mipmap_start = m_image.z_slice == 0
                        && m_image.face_index == 0
                        && m_image.array_index == 0;
    uint32_t mipmap_size = 0;

    auto except = m_input.exceptions();
    m_input.exceptions(std::ios::failbit);

    if (mipmap_start) {
        m_input.read(reinterpret_cast<char*>(&mipmap_size), sizeof mipmap_size);
    }

    update_dimensions(m_file.block_format);
    unsigned bytes_per_block = m_file.block_format.size_bits / 8;
    size_t data_size = m_image.x_blocks * m_image.y_blocks * bytes_per_block;

    m_image.data.resize(data_size);
    m_input.read(reinterpret_cast<char*>(m_image.data.data()),
                 m_image.data.size());

    if (mipmap_start) {
        size_t padlen = 3 - ((mipmap_size + 3) % 4);
        m_input.seekg(padlen, std::ios::cur);
    }

    m_input.exceptions(except);
}

KtxImageReader::KtxImageReader(const KtxFile& file, std::istream& input)
    : m_file(file),
      m_input(input)
{}

KtxImageReadIterator KtxImageReader::begin()
{
    return KtxImageReadIterator(m_file, m_input);
}

nullptr_t KtxImageReader::end()
{
    return nullptr;
}

} // ns simple_ktx
