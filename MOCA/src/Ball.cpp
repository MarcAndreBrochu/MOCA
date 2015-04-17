#include "Ball.h"
#include <exception>
#include <cmath>

using namespace std;
using namespace arma;

Ball::Ball(double radius, bool isHollow) : Solid(isHollow) {
    _radius = radius;

    // Il faut verifier si le radius est superieur a zero,
    // sinon il y a erreur de logique...
    if (radius <= 0)
        throw logic_error("Ball has zero or negative volume");
}

Ball::Ball() : Ball(1) {}
Ball::~Ball() {}

arma::mat33 Ball::getInertia() const {

    // Retourne la matrice d'inertie d'une sphere selon ses axes principaux
    double ixx = 2 * _mass * _radius * _radius / 5.0f;
    double iyy = ixx;
    double izz = ixx;

    mat inertiaTensor = {
        ixx,  0,   0,
         0,  iyy,  0,
         0,   0,  izz
    };
    inertiaTensor.reshape(3, 3);

    return inertiaTensor;
}

double Ball::getVolume() const {
    // Le volume d'une sphere est 4pi*r^3/3
    return (4 * M_PI * _radius * _radius * _radius) / 3;
}
