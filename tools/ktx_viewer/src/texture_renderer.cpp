#include <epoxy/gl.h>

#include "texture_renderer.hpp"

#include <simple_ktx.hpp>

#include <QOpenGLContext>
#include <QOpenGLFunctions>

TextureRenderer::TextureRenderer(QWidget* parent)
  : QOpenGLWidget(parent),
    m_tex_id(0),
    m_ktx(nullptr),
    m_input(nullptr)
{}

TextureRenderer::~TextureRenderer()
{
    QOpenGLContext* context = QOpenGLContext::currentContext();

    if (context != nullptr) {
        QOpenGLFunctions* f = context->functions();

        if (m_tex_id != 0) {
            f->glDeleteTextures(1, &m_tex_id);
        }
    }
}

void TextureRenderer::initializeGL()
{
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}

void TextureRenderer::resizeGL(int w, int h)
{
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    f->glViewport(0, 0, w, h);
}

void TextureRenderer::paintGL()
{
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT);

    if (m_tex_id == 0) {
        return;
    }

    f->glBindTexture(GL_TEXTURE_2D, m_tex_id);
    f->glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f (0, 0);
    glVertex3f(-1.0, -1.0, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(1.0, -1.0, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1.0, 1.0, 0.0);
    glEnd();
    f->glDisable(GL_TEXTURE_2D);
    f->glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureRenderer::change_image(const simple_ktx::KtxFile& ktx_file,
                                   unsigned mipmap, unsigned array_index,
                                   unsigned face, unsigned z_slice)
{
    if (m_input == nullptr) {
        throw std::logic_error("No input stream was specified");
    }

    m_ktx = &ktx_file;

    ktx_file.rewind_after_header(*m_input);

    for (const auto& image : ktx_file.read_images_from(*m_input)) {
        if (image.mipmap_level == mipmap
            && image.array_index == array_index
            && image.face_index == face
            && image.z_slice == z_slice) {
            load_image(image);
            update();
            return;
        }
    }

    throw std::runtime_error("Couldn't find image in KTX file");
}

void TextureRenderer::load_image(const simple_ktx::KtxImage& image)
{
    QOpenGLContext* context = QOpenGLContext::currentContext();
    if (context == nullptr) {
        throw std::runtime_error("No OpenGL context");
    }

    QOpenGLFunctions* f = context->functions();
    const simple_ktx::KtxFileHeader& header = m_ktx->header;

    if (m_tex_id != 0) {
        f->glDeleteTextures(1, &m_tex_id);
    }

    f->glGenTextures(1, &m_tex_id);
    f->glBindTexture(GL_TEXTURE_2D, m_tex_id);

    if (header.gl_type == 0) { // Compressed
        f->glCompressedTexImage2D(
            GL_TEXTURE_2D,
            0,
            header.gl_internal_format,
            image.px_width, image.px_height,
            0,
            image.data.size(),
            image.data.data()
        );
    }

    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);

    if (f->glGetError() != GL_NO_ERROR) {
        throw std::runtime_error("OpenGL error");
    }
}

void TextureRenderer::set_input(std::istream* input)
{
    m_input = input;
}
