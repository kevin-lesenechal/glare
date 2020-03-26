#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

#include <simple_ktx.hpp>
#include <fstream>

#include "mainwindow.hpp"

namespace ktx = simple_ktx;

int main(int argc, char** argv)
{
    QApplication qapp(argc, argv);

    std::string filename;
    if (qapp.arguments().size() == 1) {
        filename = QFileDialog::getOpenFileName(
            nullptr,
            QObject::tr("Open KTX texture file")
        ).toStdString();

        if (filename.empty()) {
            return 1;
        }
    } else {
        filename = qapp.arguments()[1].toStdString();
    }

    std::ifstream input;
    input.open(filename, std::ios::binary);
    if (!input) {
        QMessageBox::critical(
            nullptr,
            QObject::tr("KTX file viewer"),
            QObject::tr("Couldn't open file '%1'").arg(QString(filename.c_str()))
        );
        return 1;
    }

    try {
        ktx::KtxFile file = ktx::read_ktx_header(input);

        MainWindow mainwindow(filename, file, input);
        mainwindow.show();

        return qapp.exec();
    } catch (const std::exception& e) {
        QMessageBox::critical(
            nullptr,
            QObject::tr("KTX file viewer"),
            QObject::tr("Failed to load KTX file: %1").arg(e.what())
        );
        return 1;
    }
}
