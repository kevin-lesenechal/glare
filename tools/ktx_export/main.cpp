#include <epoxy/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <simple_ktx.hpp>

#include <Magick++.h>

#include "options.hpp"

using namespace simple_ktx;

template<typename T>
static bool is_power_2(T n)
{
    return (n & (n - T(1))) == T(0);
}

template<typename T>
static T log2_int(T n)
{
    T ret = 0;
    while (n >>= T(1)) {
        ++ret;
    }

    return ret;
}

static const char* imagemagick_format(GLenum gl_format)
{
    switch (gl_format) {
    case GL_RGB:    return "RGB";
    case GL_RGBA:   return "RGBA";
    default:
        throw std::runtime_error("Unsupported data format");
    }
}

void compress_image(Magick::Image& input,
                    GLenum data_format,
                    GLenum internal_format,
                    std::vector<uint8_t>& output)
{
    GLuint tex_id;
    GLint compressed_size;

    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    Magick::Geometry image_size = input.size();
    Magick::Blob blob;
    input.write(&blob, imagemagick_format(data_format));

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        internal_format,
        image_size.width(), image_size.height(),
        0,
        data_format,
        GL_UNSIGNED_BYTE,
        blob.data()
    );

    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error("OpenGL error");
    }

    glGetTexLevelParameteriv(
        GL_TEXTURE_2D,
        0,
        GL_TEXTURE_COMPRESSED_IMAGE_SIZE,
        &compressed_size
    );
    output.resize(compressed_size);
    glGetCompressedTexImage(GL_TEXTURE_2D, 0, output.data());

    glDeleteTextures(1, &tex_id);
}

static void on_glfw_error(int, const char* message)
{
    throw std::runtime_error(message);
}

void setup_opengl_context()
{
    if (!glfwInit()) {
        throw std::runtime_error("Coulnd't initialize GLFW");
    }

    glfwSetErrorCallback(&on_glfw_error);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(64, 64, "", nullptr, nullptr);
    if (window == nullptr) {
        throw std::runtime_error("Couldn't create GLFW window");
    }

    glfwMakeContextCurrent(window);
}

int main(int argc, char** argv)
{
    Magick::InitializeMagick(*argv);

    AppOptions options = parse_options(argc, argv);

    setup_opengl_context();

    uint32_t px_width = 0;
    uint32_t px_height = 0;
    bool first_image = true;

    for (const auto& input_file : options.input_files) {
        Magick::Image input(input_file);
        Magick::Geometry size = input.size();

        if (size.width() != size.height()) {
            throw std::runtime_error("Rectangular textures aren't supported");
        } else if (!is_power_2(size.width())) {
            throw std::runtime_error("Non-power of two textures aren't supported");
        }

        if (first_image) {
            px_width = size.width();
            px_height = size.height();
            first_image = false;
        }

        if (size.width() != px_width || size.height() != px_height) {
            throw std::runtime_error("All images must be the same size");
        }
    }

    std::ofstream out_fh;
    out_fh.exceptions(std::ios::failbit);
    out_fh.open(options.output_file, std::ios::binary | std::ios::trunc);

    KtxFile output;
    KtxFileHeader& out_h = output.header;

    if (options.flip_image) {
        output.key_values["KTXorientation"] = "S=r,T=u";
    } else {
        output.key_values["KTXorientation"] = "S=r,T=d";
    }

    if (options.merge_type == AppOptions::MergeType::Array) {
        out_h.nr_array_elements = options.input_files.size();
    } else if (options.merge_type == AppOptions::MergeType::Faces) {
        out_h.nr_faces = options.input_files.size();
    }

    if (options.make_mipmaps) {
        out_h.nr_mipmap_levels = log2_int(px_width) + 1;
    } else {
        out_h.nr_mipmap_levels = 0;
    }

    out_h.gl_internal_format = gl_tables::internal_format_value(options.format);
    out_h.pixel_width = px_width;
    out_h.pixel_height = px_height;

    output.block_format = gl_tables::format_block_size(
        out_h.gl_internal_format
    );

    GLenum data_format = gl_tables::base_format_for_internal(
        out_h.gl_internal_format
    );
    out_h.gl_base_internal_format = data_format;

    if (!output.block_format.compressed) {
        out_h.gl_type = GL_UNSIGNED_BYTE;
        out_h.gl_format = data_format;
        out_h.gl_type_size = 1;
    }

    write_ktx_header(out_fh, output);

    std::cout << "Output: " << options.output_file << std::endl;
    print_header(stdout, out_h);
    std::cout << "Images:" << std::endl;

    for (auto& image : output.write_images_to(out_fh)) {
        unsigned input_index;

        switch (options.merge_type) {
            case AppOptions::MergeType::Array:
                input_index = image.array_index;
                break;
            case AppOptions::MergeType::Faces:
                input_index = image.face_index;
                break;
            default:
                input_index = 0;
        }

        std::string input_file = options.input_files[input_index];
        Magick::Image input(input_file);
        if (options.flip_image) {
            input.flip();
        }
        input.resize(Magick::Geometry(image.px_width, image.px_height));


        if (output.block_format.compressed) {
            compress_image(input, data_format,
                           out_h.gl_internal_format, image.data);
        } else {
            magick::Blob blob;
            input.write(&blob, imagemagick_format(data_format));
            image.data.resize(blob.length());
            std::memcpy(image.data.data(), blob.data(), blob.length());
        }
        printf(" -> mipmap=%2u; %4u × %-4u px; %4u × %-4u blocks; size=0x%lx\n",
            image.mipmap_level, image.px_width, image.px_height, image.x_blocks,
            image.y_blocks, image.data.size());
    }
}
