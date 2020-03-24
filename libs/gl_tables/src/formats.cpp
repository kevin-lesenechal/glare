#include <stdexcept>
#include "gl_tables.hpp"

namespace gl_tables {

FormatBlockSize format_block_size(GLenum internal_format)
{
    switch (internal_format) {
        // 8 bits per component
    case GL_R8:
    case GL_R8_SNORM:
    case GL_R8UI:
    case GL_R8I: return {false, false, 8, 1, 1, 1};
    case GL_RG8:
    case GL_RG8_SNORM:
    case GL_RG8UI:
    case GL_RG8I: return {false, false, 16, 1, 1, 1};
    case GL_RGB8:
    case GL_RGB8_SNORM:
    case GL_RGB8UI:
    case GL_RGB8I:
    case GL_SRGB8: return {false, false, 24, 1, 1, 1};
    case GL_RGBA8:
    case GL_RGBA8_SNORM:
    case GL_RGBA8UI:
    case GL_RGBA8I:
    case GL_SRGB8_ALPHA8: return {false, false, 32, 1, 1, 1};

        // 16 bits per component
    case GL_R16:
    case GL_R16_SNORM:
    case GL_R16UI:
    case GL_R16I:
    case GL_R16F: return {false, false, 16, 1, 1, 1};
    case GL_RG16:
    case GL_RG16_SNORM:
    case GL_RG16UI:
    case GL_RG16I:
    case GL_RG16F: return {false, false, 32, 1, 1, 1};
    case GL_RGB16:
    case GL_RGB16_SNORM:
    case GL_RGB16UI:
    case GL_RGB16I:
    case GL_RGB16F: return {false, false, 48, 1, 1, 1};
    case GL_RGBA16:
    case GL_RGBA16_SNORM:
    case GL_RGBA16UI:
    case GL_RGBA16I:
    case GL_RGBA16F: return {false, false, 64, 1, 1, 1};

        // 32 bits per component
    case GL_R32UI:
    case GL_R32I:
    case GL_R32F: return {false, false, 32, 1, 1, 1};
    case GL_RG32UI:
    case GL_RG32I:
    case GL_RG32F: return {false, false, 64, 1, 1, 1};
    case GL_RGB32UI:
    case GL_RGB32I:
    case GL_RGB32F: return {false, false, 96, 1, 1, 1};
    case GL_RGBA32UI:
    case GL_RGBA32I:
    case GL_RGBA32F: return {false, false, 128, 1, 1, 1};

        // Packed
    case GL_R3_G3_B2: return {false, true, 8, 1, 1, 1};
    case GL_RGB4: return {false, true, 12, 1, 1, 1};
    case GL_RGB5: return {false, true, 16, 1, 1, 1};
    case GL_RGB565: return {false, true, 16, 1, 1, 1};
    case GL_RGB10: return {false, true, 32, 1, 1, 1};
    case GL_RGB12: return {false, true, 36, 1, 1, 1};
    case GL_RGBA2: return {false, true, 8, 1, 1, 1};
    case GL_RGBA4: return {false, true, 16, 1, 1, 1};
    case GL_RGBA12: return {false, true, 48, 1, 1, 1};
    case GL_RGB5_A1: return {false, true, 32, 1, 1, 1};
    case GL_RGB10_A2: return {false, true, 32, 1, 1, 1};
    case GL_RGB10_A2UI: return {false, true, 32, 1, 1, 1};
    case GL_R11F_G11F_B10F:
    case GL_RGB9_E5: return {false, true, 32, 1, 1, 1};

        // S3TC/DXT/BC
    case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
    case GL_COMPRESSED_SRGB_S3TC_DXT1_EXT:
    case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT: return {true, false, 64, 4, 4, 1};
    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
    case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT:
    case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT: return {true, false, 128, 4, 4, 1};

    case GL_COMPRESSED_LUMINANCE_LATC1_EXT:
    case GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT: return {true, false, 64, 4, 4, 1};
    case GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT:
    case GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT: return {true, false, 128, 4, 4, 1};

    case GL_COMPRESSED_RED_RGTC1:
    case GL_COMPRESSED_SIGNED_RED_RGTC1: return {true, false, 64, 4, 4, 1};
    case GL_COMPRESSED_RG_RGTC2:
    case GL_COMPRESSED_SIGNED_RG_RGTC2: return {true, false, 128, 4, 4, 1};

    case GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT:
    case GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT:
    case GL_COMPRESSED_RGBA_BPTC_UNORM:
    case GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM: return {true, false, 128, 4, 4, 1};

        // ETC
    case GL_ETC1_RGB8_OES:
    case GL_COMPRESSED_RGB8_ETC2:
    case GL_COMPRESSED_SRGB8_ETC2:
    case GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2:
    case GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2: return {true, false, 64, 4, 4, 1};
    case GL_COMPRESSED_RGBA8_ETC2_EAC:
    case GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC: return {true, false, 128, 4, 4, 1};

    case GL_COMPRESSED_R11_EAC:
    case GL_COMPRESSED_SIGNED_R11_EAC: return {true, false, 64, 4, 4, 1};
    case GL_COMPRESSED_RG11_EAC:
    case GL_COMPRESSED_SIGNED_RG11_EAC: return {true, false, 128, 4, 4, 1};

        // PVRTC
    case GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG:
    case GL_COMPRESSED_SRGB_PVRTC_2BPPV1_EXT:
    case GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG:
    case GL_COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV1_EXT: return {true, false, 64, 16, 8, 1};
    case GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG:
    case GL_COMPRESSED_SRGB_PVRTC_4BPPV1_EXT:
    case GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:
    case GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV1_EXT: return {true, false, 64, 8, 8, 1};
    case GL_COMPRESSED_RGBA_PVRTC_2BPPV2_IMG:
    case GL_COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV2_IMG: return {true, false, 64, 8, 4, 1};
    case GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG:
    case GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV2_IMG: return {true, false, 64, 4, 4, 1};

        // ASTC
    case GL_COMPRESSED_RGBA_ASTC_4x4_KHR:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR: return {true, false, 128, 4, 4, 1};
    case GL_COMPRESSED_RGBA_ASTC_5x4_KHR:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR: return {true, false, 128, 5, 4, 1};
    case GL_COMPRESSED_RGBA_ASTC_5x5_KHR:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR: return {true, false, 128, 5, 5, 1};
    case GL_COMPRESSED_RGBA_ASTC_6x5_KHR:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR: return {true, false, 128, 6, 5, 1};
    case GL_COMPRESSED_RGBA_ASTC_6x6_KHR:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR: return {true, false, 128, 6, 6, 1};
    case GL_COMPRESSED_RGBA_ASTC_8x5_KHR:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR: return {true, false, 128, 8, 5, 1};
    case GL_COMPRESSED_RGBA_ASTC_8x6_KHR:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR: return {true, false, 128, 8, 6, 1};
    case GL_COMPRESSED_RGBA_ASTC_8x8_KHR:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR: return {true, false, 128, 8, 8, 1};
    case GL_COMPRESSED_RGBA_ASTC_10x5_KHR:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR: return {true, false, 128, 10, 5, 1};
    case GL_COMPRESSED_RGBA_ASTC_10x6_KHR:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR: return {true, false, 128, 10, 6, 1};
    case GL_COMPRESSED_RGBA_ASTC_10x8_KHR:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR: return {true, false, 128, 10, 8, 1};
    case GL_COMPRESSED_RGBA_ASTC_10x10_KHR:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR: return {true, false, 128, 10, 10, 1};
    case GL_COMPRESSED_RGBA_ASTC_12x10_KHR:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR: return {true, false, 128, 12, 10, 1};
    case GL_COMPRESSED_RGBA_ASTC_12x12_KHR:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR: return {true, false, 128, 12, 12, 1};

    case GL_COMPRESSED_RGBA_ASTC_3x3x3_OES:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_3x3x3_OES: return {true, false, 128, 3, 3, 3};
    case GL_COMPRESSED_RGBA_ASTC_4x3x3_OES:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x3x3_OES: return {true, false, 128, 4, 3, 3};
    case GL_COMPRESSED_RGBA_ASTC_4x4x3_OES:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4x3_OES: return {true, false, 128, 4, 4, 3};
    case GL_COMPRESSED_RGBA_ASTC_4x4x4_OES:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4x4_OES: return {true, false, 128, 4, 4, 4};
    case GL_COMPRESSED_RGBA_ASTC_5x4x4_OES:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4x4_OES: return {true, false, 128, 5, 4, 4};
    case GL_COMPRESSED_RGBA_ASTC_5x5x4_OES:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5x4_OES: return {true, false, 128, 5, 5, 4};
    case GL_COMPRESSED_RGBA_ASTC_5x5x5_OES:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5x5_OES: return {true, false, 128, 5, 5, 5};
    case GL_COMPRESSED_RGBA_ASTC_6x5x5_OES:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5x5_OES: return {true, false, 128, 6, 5, 5};
    case GL_COMPRESSED_RGBA_ASTC_6x6x5_OES:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6x5_OES: return {true, false, 128, 6, 6, 5};
    case GL_COMPRESSED_RGBA_ASTC_6x6x6_OES:
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6x6_OES: return {true, false, 128, 6, 6, 6};

        // ATC
    case GL_ATC_RGB_AMD: return {true, false, 64, 4, 4, 1};
    case GL_ATC_RGBA_EXPLICIT_ALPHA_AMD:
    case GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD: return {true, false, 128, 4, 4, 1};

    default:
        throw std::runtime_error("Unknown internal format");
    }
}

GLenum base_format_for_internal(GLenum internal_format)
{
    switch (internal_format) {
        // 8 bits per component
    case GL_R8: return GL_RED;
    case GL_RG8: return GL_RG;
    case GL_RGB8: return GL_RGB;
    case GL_RGBA8: return GL_RGBA;
    case GL_R8_SNORM: return GL_RED;
    case GL_RG8_SNORM: return GL_RG;
    case GL_RGB8_SNORM: return GL_RGB;
    case GL_RGBA8_SNORM: return GL_RGBA;
    case GL_R8UI: return GL_RED;
    case GL_RG8UI: return GL_RG;
    case GL_RGB8UI: return GL_RGB;
    case GL_RGBA8UI: return GL_RGBA;
    case GL_R8I: return GL_RED;
    case GL_RG8I: return GL_RG;
    case GL_RGB8I: return GL_RGB;
    case GL_RGBA8I: return GL_RGBA;
    case GL_SRGB8: return GL_RGB;
    case GL_SRGB8_ALPHA8: return GL_RGBA;

        // 16 bits per component
    case GL_R16: return GL_RED;
    case GL_RG16: return GL_RG;
    case GL_RGB16: return GL_RGB;
    case GL_RGBA16: return GL_RGBA;
    case GL_R16_SNORM: return GL_RED;
    case GL_RG16_SNORM: return GL_RG;
    case GL_RGB16_SNORM: return GL_RGB;
    case GL_RGBA16_SNORM: return GL_RGBA;
    case GL_R16UI: return GL_RED;
    case GL_RG16UI: return GL_RG;
    case GL_RGB16UI: return GL_RGB;
    case GL_RGBA16UI: return GL_RGBA;
    case GL_R16I: return GL_RED;
    case GL_RG16I: return GL_RG;
    case GL_RGB16I: return GL_RGB;
    case GL_RGBA16I: return GL_RGBA;
    case GL_R16F: return GL_RED;
    case GL_RG16F: return GL_RG;
    case GL_RGB16F: return GL_RGB;
    case GL_RGBA16F: return GL_RGBA;

        // 32 bits per component
    case GL_R32UI: return GL_RED;
    case GL_RG32UI: return GL_RG;
    case GL_RGB32UI: return GL_RGB;
    case GL_RGBA32UI: return GL_RGBA;
    case GL_R32I: return GL_RED;
    case GL_RG32I: return GL_RG;
    case GL_RGB32I: return GL_RGB;
    case GL_RGBA32I: return GL_RGBA;
    case GL_R32F: return GL_RED;
    case GL_RG32F: return GL_RG;
    case GL_RGB32F: return GL_RGB;
    case GL_RGBA32F: return GL_RGBA;

        // Packed
    case GL_R3_G3_B2: return GL_RGB;
    case GL_RGB4: return GL_RGB;
    case GL_RGB5: return GL_RGB;
    case GL_RGB565: return GL_RGB;
    case GL_RGB10: return GL_RGB;
    case GL_RGB12: return GL_RGB;
    case GL_RGBA2: return GL_RGBA;
    case GL_RGBA4: return GL_RGBA;
    case GL_RGBA12: return GL_RGBA;
    case GL_RGB5_A1: return GL_RGBA;
    case GL_RGB10_A2: return GL_RGBA;
    case GL_RGB10_A2UI: return GL_RGBA;
    case GL_R11F_G11F_B10F: return GL_RGB;
    case GL_RGB9_E5: return GL_RGB;

        // S3TC/DXT/BC
    case GL_COMPRESSED_RGB_S3TC_DXT1_EXT: return GL_RGB;
    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT: return GL_RGBA;
    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT: return GL_RGBA;
    case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT: return GL_RGBA;
    case GL_COMPRESSED_SRGB_S3TC_DXT1_EXT: return GL_RGB;
    case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT: return GL_RGBA;
    case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT: return GL_RGBA;
    case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT: return GL_RGBA;
    case GL_COMPRESSED_LUMINANCE_LATC1_EXT: return GL_RED;
    case GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT: return GL_RG;
    case GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT: return GL_RED;
    case GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT: return GL_RG;
    case GL_COMPRESSED_RED_RGTC1: return GL_RED;
    case GL_COMPRESSED_RG_RGTC2: return GL_RG;
    case GL_COMPRESSED_SIGNED_RED_RGTC1: return GL_RED;
    case GL_COMPRESSED_SIGNED_RG_RGTC2: return GL_RG;
    case GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT: return GL_RGB;
    case GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT: return GL_RGB;
    case GL_COMPRESSED_RGBA_BPTC_UNORM: return GL_RGBA;
    case GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM: return GL_RGBA;

        // ETC
    case GL_ETC1_RGB8_OES: return GL_RGB;
    case GL_COMPRESSED_RGB8_ETC2: return GL_RGB;
    case GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2: return GL_RGBA;
    case GL_COMPRESSED_RGBA8_ETC2_EAC: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ETC2: return GL_RGB;
    case GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC: return GL_RGBA;
    case GL_COMPRESSED_R11_EAC: return GL_RED;
    case GL_COMPRESSED_RG11_EAC: return GL_RG;
    case GL_COMPRESSED_SIGNED_R11_EAC: return GL_RED;
    case GL_COMPRESSED_SIGNED_RG11_EAC: return GL_RG;

        // PVRTC
    case GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG: return GL_RGB;
    case GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG: return GL_RGB;
    case GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG: return GL_RGBA;
    case GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG: return GL_RGBA;
    case GL_COMPRESSED_RGBA_PVRTC_2BPPV2_IMG: return GL_RGBA;
    case GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG: return GL_RGBA;
    case GL_COMPRESSED_SRGB_PVRTC_2BPPV1_EXT: return GL_RGB;
    case GL_COMPRESSED_SRGB_PVRTC_4BPPV1_EXT: return GL_RGB;
    case GL_COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV1_EXT: return GL_RGBA;
    case GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV1_EXT: return GL_RGBA;
    case GL_COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV2_IMG: return GL_RGBA;
    case GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV2_IMG: return GL_RGBA;

        // ASTC
    case GL_COMPRESSED_RGBA_ASTC_4x4_KHR: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_5x4_KHR: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_5x5_KHR: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_6x5_KHR: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_6x6_KHR: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_8x5_KHR: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_8x6_KHR: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_8x8_KHR: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_10x5_KHR: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_10x6_KHR: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_10x8_KHR: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_10x10_KHR: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_12x10_KHR: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_12x12_KHR: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_3x3x3_OES: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_4x3x3_OES: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_4x4x3_OES: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_4x4x4_OES: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_5x4x4_OES: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_5x5x4_OES: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_5x5x5_OES: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_6x5x5_OES: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_6x6x5_OES: return GL_RGBA;
    case GL_COMPRESSED_RGBA_ASTC_6x6x6_OES: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_3x3x3_OES: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x3x3_OES: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4x3_OES: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4x4_OES: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4x4_OES: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5x4_OES: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5x5_OES: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5x5_OES: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6x5_OES: return GL_RGBA;
    case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6x6_OES: return GL_RGBA;

        // ATC
    case GL_ATC_RGB_AMD: return GL_RGB;
    case GL_ATC_RGBA_EXPLICIT_ALPHA_AMD: return GL_RGBA;
    case GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD: return GL_RGBA;

        // Palletized
    case GL_PALETTE4_RGB8_OES: return GL_RGB;
    case GL_PALETTE4_RGBA8_OES: return GL_RGBA;
    case GL_PALETTE4_R5_G6_B5_OES: return GL_RGB;
    case GL_PALETTE4_RGBA4_OES: return GL_RGBA;
    case GL_PALETTE4_RGB5_A1_OES: return GL_RGBA;
    case GL_PALETTE8_RGB8_OES: return GL_RGB;
    case GL_PALETTE8_RGBA8_OES: return GL_RGBA;
    case GL_PALETTE8_R5_G6_B5_OES: return GL_RGB;
    case GL_PALETTE8_RGBA4_OES: return GL_RGBA;
    case GL_PALETTE8_RGB5_A1_OES: return GL_RGBA;

        // Depth/stencil
    case GL_DEPTH_COMPONENT16: return GL_DEPTH_COMPONENT;
    case GL_DEPTH_COMPONENT24: return GL_DEPTH_COMPONENT;
    case GL_DEPTH_COMPONENT32: return GL_DEPTH_COMPONENT;
    case GL_DEPTH_COMPONENT32F: return GL_DEPTH_COMPONENT;
    case GL_DEPTH_COMPONENT32F_NV: return GL_DEPTH_COMPONENT;
    case GL_STENCIL_INDEX1: return GL_STENCIL_INDEX;
    case GL_STENCIL_INDEX4: return GL_STENCIL_INDEX;
    case GL_STENCIL_INDEX8: return GL_STENCIL_INDEX;
    case GL_STENCIL_INDEX16: return GL_STENCIL_INDEX;
    case GL_DEPTH24_STENCIL8: return GL_DEPTH_STENCIL;
    case GL_DEPTH32F_STENCIL8: return GL_DEPTH_STENCIL;
    case GL_DEPTH32F_STENCIL8_NV: return GL_DEPTH_STENCIL;

    default:
        throw std::runtime_error("Unknown internal format");
    }
}

} // ns gl_tables
