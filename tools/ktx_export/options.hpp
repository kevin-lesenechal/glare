/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include <vector>
#include <string>
#include <filesystem>

struct AppOptions
{
    enum class MergeType {
        Flat,
        Faces,
        Array
    };

    std::vector<std::string> input_files;
    std::filesystem::path output_file;
    std::string format;
    MergeType   merge_type = MergeType::Flat;
    bool        make_mipmaps = true;
    bool        y_down = false;
    bool        flip_image = true;
};

AppOptions parse_options(int argc, char** argv);
