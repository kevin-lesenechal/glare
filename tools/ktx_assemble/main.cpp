#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>

#include <glare/material/ktx_loader.hpp>
#include <gl_tables.hpp>

namespace po = boost::program_options;

using glare::KtxFileHeader;

static void usage(const char* argv0, const po::options_description& desc)
{
    std::cerr << "ktx_assemble -- Assemble KTX texture files together\n\n"
              << "Usage: " << argv0 << " [OPTION...] INPUT...\n\n"
              << "Where INPUT is a list of KTX files to merge together.\n\n"
              << "Options:\n"
              << desc << std::endl;
}

static void print_header(FILE* fh, KtxFileHeader& h)
{
    std::string type = gl_tables::type_name(h.gl_type);
    std::string format = gl_tables::format_name(h.gl_format);
    std::string in_format = gl_tables::internal_format_name(h.gl_internal_format);

    fprintf(fh, "Output KTX file:\n");
    fprintf(fh, "    Dimensions       %u × %u × %u\n",
            h.pixel_width, h.pixel_height, h.pixel_depth);
    fprintf(fh, "    Type             0x%.4x  %s  [size = %u]\n",
            h.gl_type, type.c_str(), h.gl_type_size);
    fprintf(fh, "    Format           0x%.4x  %s\n",
            h.gl_format, format.c_str());
    fprintf(fh, "    Internal format  0x%.4x  %s\n",
            h.gl_internal_format, in_format.c_str());
    fprintf(fh, "    # array elems.   %u\n",
            h.nr_array_elements);
    fprintf(fh, "    # faces          %u\n",
            h.nr_faces);
    fprintf(fh, "    # mipmap levels  %u\n",
            h.nr_mipmap_levels);
}

struct Input
{
    KtxFileHeader header;
    std::ifstream stream;
};

int main(int argc, char** argv)
{
    po::options_description desc;
    desc.add_options()
        ("help,h", "Display help and command usage")
        ("input,i",
            po::value<std::vector<std::string>>(),
            "Input KTX files to merge");
    po::positional_options_description pos_desc;
    pos_desc.add("input", -1);

    po::variables_map vars;
    po::store(
        po::command_line_parser(argc, argv)
            .options(desc)
            .positional(pos_desc)
            .run(),
        vars
    );
    po::notify(vars);

    if (vars.count("help")) {
        usage(argv[0], desc);
        return 0;
    } else if (vars.count("input") == 0) {
        std::cerr << "Please specify at least one input file." << std::endl;
        usage(argv[0], desc);
        return 1;
    }

    auto input_files = vars["input"].as<std::vector<std::string>>();

    std::vector<Input> inputs(input_files.size());

    KtxFileHeader out_header;
    std::memset(&out_header, 0, sizeof out_header);
    std::memcpy(out_header.magic, KtxFileHeader::MAGIC, sizeof out_header.magic);
    out_header.endianness  = 0x04030201;
    out_header.nr_array_elements = 0;
    out_header.nr_faces = input_files.size();

    bool first_file = true;

    unsigned i = 0;
    for (const auto& input_file : input_files) {
        KtxFileHeader& in_header = inputs[i].header;
        std::ifstream& in_fh = inputs[i].stream;

        in_fh.open(input_file, std::ios::binary);
        in_fh.read(reinterpret_cast<char*>(&in_header), sizeof in_header);

        if (std::memcmp(in_header.magic, KtxFileHeader::MAGIC,
                        sizeof in_header.magic) != 0) {
            std::cerr << input_file << ": invalid KTX magic signature" << std::endl;
            return 1;
        }

        if (in_header.nr_array_elements > 0 || in_header.nr_faces != 1) {
            std::cerr << input_file << ": arrays and multi-faces not supported"
                      << std::endl;
            return 1;
        } else if (in_header.key_value_bytes > 0) {
            std::cerr << input_file << ": extra key-values not supported"
                      << std::endl;
            return 1;
        } else if (in_header.pixel_depth > 0) {
            std::cerr << input_file << ": pixel depth > 0 not supported"
                      << std::endl;
            return 1;
        }

        if (first_file) {
            out_header.gl_type = in_header.gl_type;
            out_header.gl_type_size = in_header.gl_type_size;
            out_header.gl_format = in_header.gl_format;
            out_header.gl_internal_format = in_header.gl_internal_format;
            out_header.gl_base_internal_format = in_header.gl_base_internal_format;
            out_header.pixel_width = in_header.pixel_width;
            out_header.pixel_height = in_header.pixel_height;
            out_header.pixel_depth = in_header.pixel_depth;
            out_header.nr_mipmap_levels = in_header.nr_mipmap_levels;
            first_file = false;
        }

        if (out_header.gl_type != in_header.gl_type
            || out_header.gl_type_size != in_header.gl_type_size
            || out_header.gl_format != in_header.gl_format
            || out_header.gl_internal_format != in_header.gl_internal_format
            || out_header.gl_base_internal_format
               != in_header.gl_base_internal_format) {
            std::cerr << input_file << ": all inputs must have the same type and "
                                  "format"
                      << std::endl;
            return 1;
        }

        if (out_header.pixel_width != in_header.pixel_width
            || out_header.pixel_height != in_header.pixel_height
            || out_header.pixel_depth != in_header.pixel_depth) {
            std::cerr << input_file << ": all inputs must have the same size"
                      << std::endl;
            return 1;
        }

        if (out_header.nr_mipmap_levels != in_header.nr_mipmap_levels) {
            std::cerr << input_file << ": all inputs must have the same number "
                                  "of mipmaps"
                      << std::endl;
            return 1;
        }

        ++i;
    }

    print_header(stderr, out_header);

    std::ofstream out_fh;
    out_fh.open("out.ktx", std::ios::binary | std::ios::trunc);

    out_fh.write(reinterpret_cast<char*>(&out_header), sizeof out_header);

    unsigned level_count;
    if (out_header.nr_mipmap_levels == 0) {
        level_count = 1;
    } else {
        level_count = out_header.nr_mipmap_levels;
    }

    for (unsigned level = 0; level < level_count; ++level) {
        unsigned input_i = 0;
        for (Input& input : inputs) {
            uint32_t image_size;
            input.stream.read(
                reinterpret_cast<char*>(&image_size),
                sizeof image_size
            );

            out_fh.write(reinterpret_cast<char*>(&image_size), sizeof image_size);
            auto image_data = std::unique_ptr<char[]>(new char[image_size]);
            input.stream.read(image_data.get(), image_size);
            out_fh.write(image_data.get(), image_size);

            /*unsigned padding_length = 3 - ((image_size + 3) % 4);
            std::fill_n(
                std::ostreambuf_iterator<char>(out_fh),
                padding_length,
                '\0'
            );*/

            ++input_i;
        }
    }
}
