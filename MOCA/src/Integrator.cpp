#include "Integrator.h"


Integrator::Integrator(){}

// .........................................................
// Implementation de la methode de Simpson
// .........................................................
Integrator::simpson(int a, int b, int n) {
    int i = 0;
    double somme = 0;
    double h = 0.0;
    h = (b-a)/static_cast<float>(n);
    somme = somme + yFonct(a) + yFonct(b);
    for(i=1;i<n; i=i+2)
        somme = somme + 4*yFonct(i*h);
    for(i=2; i<n-1; i=i+2)
        somme = somme + 2*yFonct(i*h);
    return somme*h/3;
}


// .........................................................
// Implementation de la methode de Verlet
// .........................................................
/*avec les paramètres spécifiés individuellement*/
Integrator::verlet(arma::vec3 &x, arma::vec3 &v, aram::vec3 a, arma::vec3 a2, double step) {
  v += (1/2)*a*step;
  x += x + v * step;
  //Ici on trouve normalement a(t + step) -- a2 -- à partir du nouveau x
  //(en utilisant, par ex, une méthode qui donne l'accel. selon la pos pour un solide donné)
  v += v + (1/2)*a2*step;
}
/*en se servant d'une reference a l'objet que l'on veut deplacer*/
Integrator::verlet(AbstractBody &body, double step){
  body.setVelocity((body.getVelocity() + (1/2)*body.getAcceleration()*step));
  body.setPosition((body.getPosition() + body.getVelocity()*step));
  //Ici il s'agit de l'acceleration a la nouvelle position (peut avoir changee)
  body.setVelocity((body.getVelocity() + (1/2)*body.getAcceleration()*step));

}

Integrator::yFonct(double x) {
}

Integrator::~Integrator() {}
