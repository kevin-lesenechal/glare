#include "options.hpp"

#include <iostream>
#include <filesystem>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = std::filesystem;

static void usage(const char* argv0, const po::options_description& desc)
{
    std::cerr << "ktx_export -- Converts images to KTX texture files\n\n"
    << "Usage: " << argv0 << " [OPTION...] INPUT...\n\n"
    << "Where INPUT is a list of image files to merge together.\n\n"
    << "Options:\n"
    << desc << "\n"
    << "Examples:\n\n"
    << "1.\n"
    << "  $ ktx_export input.png -o output.ktx\n\n"
    << "  Creates a 2D texture containing a compressed version of the PNG image given\n"
    << "  as input into a KTX texture file; mipmaps a generated and saved.\n\n"
    << "2.\n"
    << "  $ ktx_export input.png --format=GL_RGBA -o output.ktx\n\n"
    << "  Same as example no 1, excepted a texture format is specified, here it is\n"
    << "  GL_RGBA: a non-compressed format. If no format is specified, a default \n"
    << "  compressed one is used: GL_COMPRESSED_RGBA_S3TC_DXT5_EXT.\n\n"
    << "3.\n"
    << "  $ ktx_export input_{1..6}.png --faces -o output.ktx\n\n"
    << "  Creates a cubemap texture based on six images given as input: input_1.png\n"
    << "  input_2.png, etc. and regroup them as faces of the same texture.\n\n"
    << "4.\n"
    << "  $ ktx_export input_{1..4}.png --array -o output.ktx\n\n"
    << "  Generates an array of four 2D textures given as input.\n";
}

AppOptions parse_options(int argc, char** argv)
{
    AppOptions options;

    po::options_description desc;
    desc.add_options()
        ("help,h", "Display help and command usage")
        ("input,i",
            po::value<std::vector<std::string>>(&options.input_files),
            "Input KTX files to merge")
        ("output,o",
            po::value<std::filesystem::path>(&options.output_file),
            "Output file name, if none is specified: either (1) one input file "
            "is given, then the output is the input's base name (without any "
            "extension) appended by .ktx; either (2) multiple input files are "
            "given, then the output is 'out.ktx'.")
        ("format,f",
            po::value<std::string>(&options.format)
                ->default_value("GL_COMPRESSED_RGBA_S3TC_DXT5_EXT"),
            "Output texture internal format. If this refers to a compressed "
            "format, this type and format of the data will be automatically "
            "determined; otherwise, it must be specified.")
        ("data-format",
            "Specifies the data format contained in the texture; for compressed "
            "textures, this option is not needed, if it is supplied, it must "
            "match with the compression format's data format.")
        ("data-type",
            "Specifies the data type contained in the texture; for compressed "
            "textures, this option is not needed, if it is supplied, it must "
            "match with the compression format's data type.")
        ("faces", "Merge images as faces.")
        ("array", "Merge images as array elements.")
        ("no-mipmaps", "Do not generate mipmaps.")
        ("y-downward",
            "Use 'S=r,T=d' coordinate system, making the origin 0,0 in the top "
            "left; OpenGL expect the origin to be in the bottom left.");
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
        std::exit(0);
    }

    if (options.input_files.empty()) {
        std::cerr << "No input file specified.\n";
        usage(argv[0], desc);
        std::exit(1);
    } else if (vars.count("faces") && vars.count("array")) {
        std::cerr << "--faces and --array are mutually exclusive.\n";
        usage(argv[0], desc);
        std::exit(1);
    }

    if (vars.count("output") == 0) {
        if (options.input_files.size() == 1) {
            options.output_file =
                fs::path(options.input_files[0]).replace_extension(".ktx");
        } else {
            options.output_file = "out.ktx";
        }
    }

    if (vars.count("faces")) {
        options.merge_type = AppOptions::MergeType::Faces;
    } else if (vars.count("array")) {
        options.merge_type = AppOptions::MergeType::Array;
    } else {
        options.merge_type = AppOptions::MergeType::Flat;
    }

    if (options.merge_type == AppOptions::MergeType::Flat
        && options.input_files.size() > 1) {
        std::cerr << "Please specify --faces or --array for multiple inputs.\n";
        usage(argv[0], desc);
        std::exit(1);
    }

    options.make_mipmaps = vars.count("no-mipmaps") == 0;
    options.flip_image = vars.count("y-downwards") == 0;

    return options;
}
