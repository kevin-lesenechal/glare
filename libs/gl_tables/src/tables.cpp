#include <cstring>
#include "gl_tables.hpp"

#define ENTRY(name) {#name, name}

namespace gl_tables {

namespace {

struct TableEntry {
    const char* name;
    GLenum      value;
};

TableEntry g_types[] = {
    ENTRY(GL_UNSIGNED_BYTE),
    ENTRY(GL_BYTE),
    ENTRY(GL_UNSIGNED_SHORT),
    ENTRY(GL_SHORT),
    ENTRY(GL_UNSIGNED_INT),
    ENTRY(GL_INT),
    ENTRY(GL_FLOAT),
    ENTRY(GL_UNSIGNED_BYTE_3_3_2),
    ENTRY(GL_UNSIGNED_BYTE_2_3_3_REV),
    ENTRY(GL_UNSIGNED_SHORT_5_6_5),
    ENTRY(GL_UNSIGNED_SHORT_5_6_5_REV),
    ENTRY(GL_UNSIGNED_SHORT_4_4_4_4),
    ENTRY(GL_UNSIGNED_SHORT_4_4_4_4_REV),
    ENTRY(GL_UNSIGNED_SHORT_5_5_5_1),
    ENTRY(GL_UNSIGNED_SHORT_1_5_5_5_REV),
    ENTRY(GL_UNSIGNED_INT_8_8_8_8),
    ENTRY(GL_UNSIGNED_INT_8_8_8_8_REV),
    ENTRY(GL_UNSIGNED_INT_10_10_10_2),
    ENTRY(GL_UNSIGNED_INT_2_10_10_10_REV),
    {nullptr, 0}
};

TableEntry g_formats[] = {
    ENTRY(GL_RED),
    ENTRY(GL_RG),
    ENTRY(GL_RGB),
    ENTRY(GL_BGR),
    ENTRY(GL_RGBA),
    ENTRY(GL_BGRA),
    ENTRY(GL_RED_INTEGER),
    ENTRY(GL_RG_INTEGER),
    ENTRY(GL_RGB_INTEGER),
    ENTRY(GL_BGR_INTEGER),
    ENTRY(GL_RGBA_INTEGER),
    ENTRY(GL_BGRA_INTEGER),
    ENTRY(GL_STENCIL_INDEX),
    ENTRY(GL_DEPTH_COMPONENT),
    ENTRY(GL_DEPTH_STENCIL),
    {nullptr, 0}
};

TableEntry g_internal_formats[] = {
    ENTRY(GL_RED),
    ENTRY(GL_LUMINANCE),
    ENTRY(GL_ALPHA),
    ENTRY(GL_R8),
    ENTRY(GL_R8_SNORM),
    ENTRY(GL_R16),
    ENTRY(GL_R16_SNORM),
    ENTRY(GL_R16F),
    ENTRY(GL_R32F),
    ENTRY(GL_R8I),
    ENTRY(GL_R8UI),
    ENTRY(GL_R16I),
    ENTRY(GL_R16UI),
    ENTRY(GL_R32I),
    ENTRY(GL_R32UI),
    ENTRY(GL_RG),
    ENTRY(GL_LUMINANCE_ALPHA),
    ENTRY(GL_RG8),
    ENTRY(GL_RG8_SNORM),
    ENTRY(GL_RG16),
    ENTRY(GL_RG16_SNORM),
    ENTRY(GL_RG16F),
    ENTRY(GL_RG32F),
    ENTRY(GL_RG8I),
    ENTRY(GL_RG8UI),
    ENTRY(GL_RG16I),
    ENTRY(GL_RG16UI),
    ENTRY(GL_RG32I),
    ENTRY(GL_RG32UI),
    ENTRY(GL_RGB),
    ENTRY(GL_R3_G3_B2),
    ENTRY(GL_RGB4),
    ENTRY(GL_RGB5),
    ENTRY(GL_RGB8),
    ENTRY(GL_RGB8_SNORM),
    ENTRY(GL_RGB10),
    ENTRY(GL_RGB12),
    ENTRY(GL_RGB16_SNORM),
    ENTRY(GL_SRGB8),
    ENTRY(GL_RGB16F),
    ENTRY(GL_RGB32F),
    ENTRY(GL_R11F_G11F_B10F),
    ENTRY(GL_RGB9_E5),
    ENTRY(GL_RGB8I),
    ENTRY(GL_RGB8UI),
    ENTRY(GL_RGB16I),
    ENTRY(GL_RGB16UI),
    ENTRY(GL_RGB32I),
    ENTRY(GL_RGB32UI),
    ENTRY(GL_RGBA),
    ENTRY(GL_RGBA2),
    ENTRY(GL_RGBA4),
    ENTRY(GL_RGB5_A1),
    ENTRY(GL_RGBA8),
    ENTRY(GL_RGBA8_SNORM),
    ENTRY(GL_RGB10_A2),
    ENTRY(GL_RGB10_A2UI),
    ENTRY(GL_RGBA12),
    ENTRY(GL_RGBA16),
    ENTRY(GL_SRGB8_ALPHA8),
    ENTRY(GL_RGBA16F),
    ENTRY(GL_RGBA32F),
    ENTRY(GL_RGBA8I),
    ENTRY(GL_RGBA8UI),
    ENTRY(GL_RGBA16I),
    ENTRY(GL_RGBA16UI),
    ENTRY(GL_RGBA32I),
    ENTRY(GL_RGBA32UI),
    ENTRY(GL_DEPTH_COMPONENT),
    ENTRY(GL_DEPTH_STENCIL),
    ENTRY(GL_COMPRESSED_RED),
    ENTRY(GL_COMPRESSED_RED_RGTC1),
    ENTRY(GL_COMPRESSED_SIGNED_RED_RGTC1),
    ENTRY(GL_COMPRESSED_R11_EAC),
    ENTRY(GL_COMPRESSED_SIGNED_R11_EAC),
    ENTRY(GL_COMPRESSED_RG),
    ENTRY(GL_COMPRESSED_RG_RGTC2),
    ENTRY(GL_COMPRESSED_SIGNED_RG_RGTC2),
    ENTRY(GL_COMPRESSED_RG11_EAC),
    ENTRY(GL_COMPRESSED_SIGNED_RG11_EAC),
    ENTRY(GL_COMPRESSED_RGB),
    ENTRY(GL_COMPRESSED_SRGB),
    ENTRY(GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT),
    ENTRY(GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT),
    ENTRY(GL_COMPRESSED_RGB_S3TC_DXT1_EXT),
    ENTRY(GL_COMPRESSED_RGB8_ETC2),
    ENTRY(GL_COMPRESSED_SRGB8_ETC2),
    ENTRY(GL_ETC1_RGB8_OES),
    ENTRY(GL_COMPRESSED_RGBA),
    ENTRY(GL_COMPRESSED_SRGB_ALPHA),
    ENTRY(GL_COMPRESSED_RGBA_BPTC_UNORM),
    ENTRY(GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM),
    ENTRY(GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT),
    ENTRY(GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT),
    ENTRY(GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT),
    ENTRY(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT),
    ENTRY(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT),
    ENTRY(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT),
    ENTRY(GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2),
    ENTRY(GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2),
    ENTRY(GL_COMPRESSED_RGBA8_ETC2_EAC),
    ENTRY(GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC),
    {nullptr, 0}
};

} // ns

std::string entry_name(Table table, GLenum value)
{
    TableEntry* entries;

    switch (table) {
        case Table::Types:          entries = g_types; break;
        case Table::Format:         entries = g_formats; break;
        case Table::InternalFormat: entries = g_internal_formats; break;
        default:
            std::abort();
    }

    for (unsigned i = 0; entries[i].name != nullptr; ++i) {
        if (entries[i].value == value) {
            return std::string(entries[i].name);
        }
    }

    return "???";
}

GLenum entry_value(Table table, const std::string& name)
{
    TableEntry* entries;

    switch (table) {
    case Table::Types:          entries = g_types; break;
    case Table::Format:         entries = g_formats; break;
    case Table::InternalFormat: entries = g_internal_formats; break;
    default:
        std::abort();
    }

    for (unsigned i = 0; entries[i].name != nullptr; ++i) {
        if (std::strcmp(entries[i].name, name.c_str()) == 0) {
            return entries[i].value;
        }
    }

    return 0;
}

} // gl_tables
