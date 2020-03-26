#pragma once

#include <QOpenGLWidget>

#include <simple_ktx.hpp>

class TextureRenderer : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit TextureRenderer(QWidget* parent = nullptr);

    ~TextureRenderer() override;

    void change_image(const simple_ktx::KtxFile& ktx_file,
                      unsigned mipmap, unsigned array_index,
                      unsigned face, unsigned z_slice);

    void set_input(std::istream* input);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void load_image(const simple_ktx::KtxImage& image);

private:
    GLuint m_tex_id;
    const simple_ktx::KtxFile* m_ktx;
    std::istream* m_input;
};
