#include <iostream>
#include <QApplication>
#include <QWindow>
#include <QSurfaceFormat>

#include "GLWidget.h"

using namespace std;

int main(int argc, char **argv) {

    // Pour une raison quelconque, si on place ca apres la creation du QApplication
    // on a un crash... C'EST DE LA MAGIE DE PAS TOUCHER.
    QSurfaceFormat nformat;
    nformat.setDepthBufferSize(24);
    nformat.setMajorVersion(3);
    nformat.setMinorVersion(3);
    nformat.setSamples(4);
    nformat.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(nformat);

    QApplication app(argc, argv);
    app.setOrganizationName("Marc-André Brochu & Ludovic Bilodeau-Laflamme");
    app.setApplicationName("MOCA test");
    app.setApplicationDisplayName(app.applicationName());

    GLWidget w;
    w.show();

    return app.exec();
}
