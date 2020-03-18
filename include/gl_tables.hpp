#pragma once

#include <epoxy/gl.h>
#include <string>

namespace gl_tables {

enum class Table {
    Types,
    Format,
    InternalFormat
};

std::string entry_name(Table table, GLenum value);

inline std::string type_name(GLenum value)
{
    return entry_name(Table::Types, value);
}

inline std::string format_name(GLenum value)
{
    return entry_name(Table::Format, value);
}

inline std::string internal_format_name(GLenum value)
{
    return entry_name(Table::InternalFormat, value);
}

struct FormatBlockSize
{
    uint8_t size_bits;
    uint8_t width;
    uint8_t height;
    uint8_t depth;
};

FormatBlockSize format_block_size(GLenum internal_format);

} // ns gl_tables
