#include "mainwindow.hpp"

#include <QFileInfo>
#include <QTimer>

#include <gl_tables.hpp>

MainWindow::MainWindow(const std::string& filename,
                       const simple_ktx::KtxFile& ktx_file,
                       std::istream& input)
  : m_ktx(ktx_file),
    m_header(m_ktx.header),
    m_input(input)
{
    setupUi(this);

    QString base_name = QFileInfo(filename.c_str()).fileName();
    setWindowTitle(tr("%1 — KTX viewer").arg(base_name));

    QFont monospace("monospace");

    this->dimensions->setText(
        QString("%1 × %2 × %3")
        .arg(m_header.pixel_width)
        .arg(m_header.pixel_height)
        .arg(m_header.pixel_depth)
    );
    this->nr_mipmap_levels->setText(QString::number(m_header.nr_mipmap_levels));
    this->nr_array_elems->setText(QString::number(m_header.nr_array_elements));
    this->nr_faces->setText(QString::number(m_header.nr_faces));

    bool compressed = m_header.gl_type == 0;

    std::string internal_format = gl_tables::internal_format_name(
        m_header.gl_internal_format
    );
    if (internal_format == "???") {
        this->internal_format->setText(
            "0x" + QString::number(m_header.gl_internal_format, 16)
        );
    } else {
        this->internal_format->setText(
            QString(internal_format.c_str())
            + " [0x" + QString::number(m_header.gl_internal_format, 16) + "]"
        );
    }
    this->internal_format->setFont(monospace);

    std::string base_format_name = gl_tables::format_name(
        m_header.gl_base_internal_format
    );
    this->base_internal_format->setText(
        QString("%1 [0x%2]")
            .arg(base_format_name.c_str())
            .arg(QString::number(m_header.gl_base_internal_format, 16))
    );

    GLenum base_format = gl_tables::base_format_for_internal(
        m_header.gl_internal_format
    );

    if (compressed) {
        this->data_format->setText("n/a (" + tr("compressed") + ")");
        this->data_type->setText("n/a (" + tr("compressed") + ")");
    } else {
        std::string format = gl_tables::format_name(m_header.gl_format);
        this->data_format->setText(
            QString("%1 [0x%2]")
            .arg(format.c_str())
            .arg(QString::number(m_header.gl_format, 16))
        );
        this->data_format->setFont(monospace);

        std::string type = gl_tables::type_name(m_header.gl_type);
        this->data_type->setText(
            QString("%1 [0x%2]")
                .arg(type.c_str())
                .arg(QString::number(m_header.gl_type, 16))
        );
        this->data_type->setFont(monospace);
    }

    this->key_values->setFont(monospace);

    for (const auto& kv : m_ktx.key_values) {
        QTreeWidgetItem* item = new QTreeWidgetItem(
            QStringList() << QString(kv.first.c_str())
                          << QString(kv.second.c_str())
        );

        this->key_values->addTopLevelItem(item);
    }

    this->mipmap_level->setEnabled(m_header.nr_mipmap_levels > 0);
    this->array_index->setEnabled(m_header.nr_array_elements > 0);
    this->face_index->setEnabled(m_header.nr_faces > 1);
    this->z_slice->setEnabled(m_header.pixel_depth > 0);
    this->alpha_mode->setEnabled(base_format == GL_RGBA);

    if (m_header.nr_mipmap_levels > 0) {
        this->mipmap_level->setRange(0, m_header.nr_mipmap_levels - 1);
    }
    if (m_header.nr_array_elements > 0) {
        this->array_index->setRange(0, m_header.nr_array_elements - 1);
    }
    if (m_header.nr_faces > 0) {
        this->face_index->setRange(0, m_header.nr_faces - 1);
    }
    if (m_header.pixel_depth > 0) {
        this->z_slice->setRange(0, m_header.pixel_depth - 1);
    }

    if (m_header.pixel_width <= 1024 && m_header.pixel_height <= 1024) {
        this->view->resize(m_header.pixel_width, m_header.pixel_height);
    }

    this->view->set_input(&input);

    connect(this->mipmap_level, &QSlider::valueChanged,
            this, &MainWindow::change_image);
    connect(this->array_index, qOverload<int>(&QSpinBox::valueChanged),
            this, &MainWindow::change_image);
    connect(this->face_index, qOverload<int>(&QSpinBox::valueChanged),
            this, &MainWindow::change_image);
    connect(this->z_slice, qOverload<int>(&QSpinBox::valueChanged),
            this, &MainWindow::change_image);
    connect(this->alpha_mode, qOverload<int>(&QComboBox::currentIndexChanged),
            this, &MainWindow::change_image);

    QTimer::singleShot(0, this, &MainWindow::change_image);
}

void MainWindow::change_image()
{
    unsigned mipmap = this->mipmap_level->value();
    unsigned array_index = this->array_index->value();
    unsigned face = this->face_index->value();
    unsigned z_slice = this->z_slice->value();

    this->view->set_alpha_mode(
        static_cast<TextureRenderer::AlphaMode>(this->alpha_mode->currentIndex())
    );
    this->view->change_image(m_ktx, mipmap, array_index, face, z_slice);
}
