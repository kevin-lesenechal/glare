/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <epoxy/gl.h>
#include <vector>

namespace glare {

enum class BufferType : GLint {
    VertexAttr          = GL_ARRAY_BUFFER,
    AtomicCounter       = GL_ATOMIC_COUNTER_BUFFER,
    CopyRead            = GL_COPY_READ_BUFFER,
    CopyWrite           = GL_COPY_WRITE_BUFFER,
    DispatchIndirect    = GL_DISPATCH_INDIRECT_BUFFER,
    DrawIndirect        = GL_DRAW_INDIRECT_BUFFER,
    ElementArray        = GL_ELEMENT_ARRAY_BUFFER,
    PixelPack           = GL_PIXEL_PACK_BUFFER,
    PixelUnpack         = GL_PIXEL_UNPACK_BUFFER,
    Query               = GL_QUERY_BUFFER,
    ShaderStorage       = GL_SHADER_STORAGE_BUFFER,
    Texture             = GL_TEXTURE_BUFFER,
    TransformFeedback   = GL_TRANSFORM_FEEDBACK_BUFFER,
    Uniform             = GL_UNIFORM_BUFFER,
};

enum class BufferUsage : GLenum {
    StreamDraw  = GL_STREAM_DRAW,
    StreamRead  = GL_STREAM_READ,
    StreamCopy  = GL_STREAM_COPY,
    StaticDraw  = GL_STATIC_DRAW,
    StaticRead  = GL_STATIC_READ,
    StaticCopy  = GL_STATIC_COPY,
    DynamicDraw = GL_DYNAMIC_DRAW,
    DynamicRead = GL_DYNAMIC_READ,
    DynamicCopy = GL_DYNAMIC_COPY,
};

template<typename T, BufferType Type>
class Buffer
{
public:
    Buffer()
      : m_last_usage(BufferUsage::StaticDraw),
        m_value_count(0)
    {
        glGenBuffers(1, &m_id);
    }

    explicit Buffer(BufferUsage usage, const std::vector<T>& data)
      : Buffer()
    {
        upload(usage, data);
    }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&& other) noexcept
      : m_id(other.m_id),
        m_last_usage(other.m_last_usage),
        m_value_count(other.m_value_count)
    {
        other.m_id = 0;
    }

    Buffer& operator=(Buffer&& other) noexcept
    {
        if (&other != this) {
            glDeleteBuffers(1, &m_id);
            m_id = other.m_id;
            other.m_id = 0;
            m_last_usage = other.m_last_usage;
            m_value_count = other.m_value_count;
        }
        return *this;
    }

    ~Buffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void bind()
    {
        glBindBuffer(static_cast<GLenum>(Type), m_id);
    }

    void upload(BufferUsage usage, const T* data, size_t value_count)
    {
        bind();
        glBufferData(
            static_cast<GLenum>(Type),
            value_count * sizeof (T),
            data,
            static_cast<GLenum>(usage)
        );
        m_last_usage  = usage;
        m_value_count = value_count;
    }

    void upload(BufferUsage usage, const std::vector<T>& data)
    {
        upload(usage, data.data(), data.size());
    }

    void upload(const std::vector<T>& data)
    {
        upload(m_last_usage, data);
    }

    [[nodiscard]] size_t value_count() const noexcept { return m_value_count; }

private:
    GLuint      m_id;
    BufferUsage m_last_usage;
    size_t      m_value_count;
};

template<typename T>
using VertexBuffer = Buffer<T, BufferType::VertexAttr>;

template<typename T>
using ElementArrayBuffer = Buffer<T, BufferType::ElementArray>;

} // ns glare
