#pragma once

#include <QOpenGLWidget>

#include <simple_ktx.hpp>

class TextureRenderer : public QOpenGLWidget
{
    Q_OBJECT
public:
    enum AlphaMode {
        Discard = 0,
        Display = 1,
        DisplayAlone = 2
    };

    explicit TextureRenderer(QWidget* parent = nullptr);

    ~TextureRenderer() override;

    void change_image(const simple_ktx::KtxFile& ktx_file,
                      unsigned mipmap, unsigned array_index,
                      unsigned face, unsigned z_slice);

    void set_input(std::istream* input);
    void set_alpha_mode(AlphaMode alpha_mode)
    { m_alpha_mode = alpha_mode; }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void load_image(const simple_ktx::KtxImage& image);

private:
    GLuint m_tex_id;
    GLuint m_bg_tex;
    const simple_ktx::KtxFile* m_ktx;
    std::istream* m_input;
    AlphaMode m_alpha_mode;
};
