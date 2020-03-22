/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

namespace glare {

struct Size1D
{
    unsigned width;

    explicit Size1D(unsigned width)
      : width(width)
    {}

    Size1D(const Size1D&) = default;
    Size1D& operator=(const Size1D&) = default;
};

struct Size2D
{
    unsigned width;
    unsigned height;

    explicit Size2D(unsigned width, unsigned height)
      : width(width),
        height(height)
    {}

    Size2D(const Size2D&) = default;
    Size2D& operator=(const Size2D&) = default;
};

struct Size3D
{
    unsigned width;
    unsigned height;
    unsigned depth;

    explicit Size3D(unsigned width, unsigned height, unsigned depth)
      : width(width),
        height(height),
        depth(depth)
    {}

    Size3D(const Size3D&) = default;
    Size3D& operator=(const Size3D&) = default;
};

struct AnySize
{
    union {
        Size1D size_1d;
        Size2D size_2d;
        Size3D size_3d;
    };

    enum class Tag {
        Size1D,
        Size2D,
        Size3D
    };

    Tag tag;

    AnySize(Size1D size)
      : size_1d(size), tag(Tag::Size1D)
    {}

    AnySize(Size2D size)
        : size_2d(size), tag(Tag::Size2D)
    {}

    AnySize(Size3D size)
        : size_3d(size), tag(Tag::Size3D)
    {}
};

} // glare
