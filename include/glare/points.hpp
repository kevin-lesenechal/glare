/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

namespace glare {

struct Point1D
{
    unsigned x;

    explicit Point1D(unsigned x)
        : x(x)
    {}

    Point1D(const Point1D&) = default;
    Point1D& operator=(const Point1D&) = default;
};

struct Point2D
{
    unsigned x;
    unsigned y;

    explicit Point2D(unsigned x, unsigned y)
        : x(x),
          y(y)
    {}

    Point2D(const Point2D&) = default;
    Point2D& operator=(const Point2D&) = default;
};

struct Point3D
{
    unsigned x;
    unsigned y;
    unsigned z;

    explicit Point3D(unsigned x, unsigned y, unsigned z)
        : x(x),
          y(y),
          z(z)
    {}

    Point3D(const Point3D&) = default;
    Point3D& operator=(const Point3D&) = default;
};

struct AnyPoint
{
    union {
        Point1D point_1d;
        Point2D point_2d;
        Point3D point_3d;
    };

    enum class Tag {
        Point1D,
        Point2D,
        Point3D
    };

    Tag tag;

    AnyPoint(Point1D point)
        : point_1d(point), tag(Tag::Point1D)
    {}

    AnyPoint(Point2D point)
        : point_2d(point), tag(Tag::Point2D)
    {}

    AnyPoint(Point3D point)
        : point_3d(point), tag(Tag::Point3D)
    {}
};

} // glare
