#pragma once

#include <QWidget>

#include <simple_ktx.hpp>
#include <istream>

#include "ui_mainwindow.h"

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT
public:
    MainWindow(const std::string& filename,
               const simple_ktx::KtxFile& ktx_file,
               std::istream& input);

private slots:
    void change_image();

private:
    const simple_ktx::KtxFile& m_ktx;
    const simple_ktx::KtxFileHeader& m_header;
    std::istream& m_input;
};
