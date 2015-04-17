#include "Integrator.h"

using namespace Integrator;

// .........................................................
// Implementation de la methode de Simpson
// .........................................................
Simpson::Simpson(int a, int b, int n) {
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
Simpson::yFonct(float x) {
}
Simpson::~Simpson() {}



// .........................................................
// Implementation de la methode de Verlet
// .........................................................
Verlet::Verlet() {}
Verlet::~Verlet() {}
