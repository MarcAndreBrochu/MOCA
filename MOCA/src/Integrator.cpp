#include "Integrator.h"
#include "AbstractBody.h"

using namespace arma;

Integrator::Integrator() {}
Integrator::~Integrator() {}

// .........................................................
// Implementation de la methode de Simpson
// .........................................................
float Integrator::simpson(int a, int b, int n) {

    int i = 0;
    double somme = 0;
    double h = 0.0;

    h = (b - a) / static_cast<float>(n);
    somme = somme + yFonct(a) + yFonct(b);

    for (i = 1; i < n; i = i + 2)
        somme = somme + 4 * yFonct(i * h);
    for (i = 2; i < n - 1; i = i + 2)
        somme = somme + 2 * yFonct(i * h);

    return somme * h / 3;
}
float Integrator::yFonct(double x) { return x; }

// .........................................................
// Implementation de la methode de Verlet
// .........................................................
/*avec les paramètres spécifiés individuellement*/
Integrator::Status Integrator::verlet(const vec3 &x, const vec3 &v, const vec3 &a, const vec3 &a2, double step) {

    Status s;
    s.p = x;
    s.v = v;
  //s.a = ?? // on n'utilise pas s.a dans l'imp de Verlet (!!)

    s.v += (1/2.0f) * a * step;
    s.p += s.v * step;
    //Ici on trouve normalement a(t + step) -- a2 -- à partir du nouveau x
    //(en utilisant, par ex, une méthode qui donne l'accel. selon la pos pour un solide donné)
    s.v += (1/2.0f) * a2 * step;

    return s;
}
/*en se servant d'une reference a l'objet que l'on veut deplacer*/
Integrator::Status Integrator::verlet(const AbstractBody &body, double step) {

    Status s;
    vec3 a = body.getAcceleration();

    s.v = (body.getVelocity() + (1/2.0f) * a * step);
    s.p = (body.getPosition() + s.v * step);
    //Ici il s'agit de l'acceleration a la nouvelle position (peut avoir changee)
    s.v = (s.v + (1/2.0f) * a * step);

    return s;
}
