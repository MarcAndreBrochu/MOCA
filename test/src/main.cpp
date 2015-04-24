#include <iostream>
#include <QApplication>
#include <QSurfaceFormat>
#include <QWindow>
#include <QGLFormat>

#include "glwidget.h"

using namespace std;

int main(int argc, char **argv) {

    QApplication app(argc, argv);
    app.setOrganizationName("Marc-André Brochu & Ludovic Bilodeau-Laflamme");
    app.setApplicationName("MOCA test");
    app.setApplicationDisplayName(app.applicationName());

    QGLFormat glFormat;
    glFormat.setVersion(3, 3);
    glFormat.setProfile(QGLFormat::CoreProfile);
    glFormat.setSampleBuffers(true);

    GLWidget w(glFormat);
    w.show();

    return app.exec();
}
