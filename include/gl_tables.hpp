/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

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

GLenum entry_value(Table table, const std::string& name);

inline GLenum type_value(const std::string& name)
{
    return entry_value(Table::Types, name);
}

inline GLenum format_value(const std::string& name)
{
    return entry_value(Table::Format, name);
}

inline GLenum internal_format_value(const std::string& name)
{
    return entry_value(Table::InternalFormat, name);
}

struct FormatBlockSize
{
    bool    compressed;
    bool    packed;
    uint8_t size_bits;
    uint8_t width;
    uint8_t height;
    uint8_t depth;
};

FormatBlockSize format_block_size(GLenum internal_format);

} // ns gl_tables
