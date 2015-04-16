#include <iostream>
#include <MOCA/MOCA.h>
#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QWindow>

#include <MOCA/MOCA.h>

using namespace std;

int main(int argc, char **argv) {

    QGuiApplication app(argc, argv);
    app.setOrganizationName("Marc-André Brochu & Ludovic Bilodeau-Laflamme");
    app.setApplicationName("MOCA test");
    app.setApplicationDisplayName(app.applicationName());

    QWindow window;
    window.resize(640, 480);
    window.show();

    World world;

    return app.exec();
}
