#include <iostream>
#include <MOCA/MOCA.h>
#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QWindow>

#include <MOCA/MOCA.h>
#include <MOCA/Utils.h>

using namespace std;

int main(int argc, char **argv) {

    QGuiApplication app(argc, argv);
    app.setOrganizationName("Marc-André Brochu & Ludovic Bilodeau-Laflamme");
    app.setApplicationName("MOCA test");
    app.setApplicationDisplayName(app.applicationName());
    
    World world;
    Particle particlel;
    particlel.setMass(1);
    particlel.setPosition(0, 10, 0);
    particlel.setVelocity(0, 0, 0);

    arma::vec3 accelGravite;
    accelGravite[0] = MOCA_METERS_TO_PIXELS(0);
    accelGravite[1] = MOCA_METERS_TO_PIXELS(-9.8);
    accelGravite[2] = MOCA_METERS_TO_PIXELS(0);

    world.addBody(&particlel);
    world.applyAcceleration(accelGravite);
//    particlel.applyForce(accelGravite);

    double fixedDt = 1.0f / 60.0f;
    for (int i = 0; i < 600; i++) {

        cout << fixedDt * i << "\t" << particlel.getPosition()[1] << "\t" << particlel.getVelocity()[1] << endl;
        world.updateWorld(fixedDt);
    }

    return app.exec();
}
