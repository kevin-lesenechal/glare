/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <epoxy/gl.h>

#include "buffer.hpp"

namespace glare {

template<typename T>
constexpr GLenum gl_type_mapping()
{
    if (std::is_same_v<T, double>) return GL_DOUBLE;
    if (std::is_same_v<T, float>) return GL_FLOAT;
}

class VertexArray
{
public:
    VertexArray();

    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    void bind();
    void unbind();
    void enable(unsigned location);
    void disable(unsigned location);

    template<typename T, BufferType BuffType>
    void attach(unsigned location,
                Buffer<T, BuffType>& buffer,
                unsigned value_count,
                unsigned stride = 0,
                size_t offset = 0,
                bool normalized = false)
    {
        GLenum type = gl_type_mapping<T>();

        bind();
        buffer.bind();

        if constexpr (std::is_integral_v<T>) {
            glVertexAttribIPointer(location, value_count, type, stride,
                                   reinterpret_cast<void*>(offset));
        } else if constexpr (std::is_same_v<T, double>) {
            glVertexAttribLPointer(location, value_count, type, stride,
                                   reinterpret_cast<void*>(offset));
        } else {
            glVertexAttribPointer(location, value_count, type, normalized, stride,
                                  reinterpret_cast<void*>(offset));
        }

        enable(location);
    }

    template<typename T>
    void attach(ElementArrayBuffer<T>& ebo)
    {
        if (ext::has_dsa) {
            glVertexArrayElementBuffer(m_id, ebo.id());
        } else {
            bind();
            ebo.bind();
            unbind();
        }
    }

    ~VertexArray() noexcept;

private:
    GLuint m_id;
};

} // ns glare
