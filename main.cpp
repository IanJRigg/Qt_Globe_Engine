#include "mainwindow.h"

#include <QApplication>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);
    MainWindow window;
    window.show();

    return a.exec();
}
