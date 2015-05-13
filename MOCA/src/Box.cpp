#include "Box.h"
#include <exception>

#define _USE_MATH_DEFINES
#include <cmath>


using namespace std;
using namespace arma;

Box::Box(double dx, double dy, double dz, bool isHollow) : Solid(isHollow) {

    _dimensionX = dx;
    _dimensionY = dy;
    _dimensionZ = dz;

    // Il faut s'assurer qu'il n'y a aucune dimension inferieure
    // ou egale a zero, sinon il y a un probleme de logique...
    if (dx <= 0 || dy <= 0 || dz <= 0)
        throw logic_error("Box has zero or negative volume");

    _listVertices = getLocalVertices();
}
Box::Box() : Box(1, 1, 1) {}
Box::~Box() {}

arma::mat33 Box::getInertia() const {

    // Les composantes de la matrice d'inertie sur les axes principaux
    // revient aux produits d'inertie principaux pour chaque axe. Pour
    // une boite, cela reviens a une matrice du genre (pour un solide plein):
    //
    // M(b^2 + c^2)/12        0                0
    //       0          M(a^2 + c^2)/12        0
    //       0                0          M(a^2 + b^2)/12

    double ixx = _mass * (_dimensionY * _dimensionY + _dimensionZ * _dimensionZ) / 12.0f;
    double iyy = _mass * (_dimensionX * _dimensionX + _dimensionZ * _dimensionZ) / 12.0f;
    double izz = _mass * (_dimensionX * _dimensionX + _dimensionY * _dimensionY) / 12.0f;

    mat inertiaTensor = {
        ixx,  0,   0,
         0,  iyy,  0,
         0,   0,  izz
    };
    inertiaTensor.reshape(3, 3);

    return inertiaTensor;
}

double Box::getVolume() const {
    // Le volume d'une boite est bien simplement toutes ses dimensions
    // multipliees ensemble
    return _dimensionX * _dimensionY * _dimensionZ;
}

arma::vec3 Box::getWorldXAxis() const{
    double phi = _angularPosition(0)*M_PI/180;
    double theta = _angularPosition(1)*M_PI/180;
    double psi = _angularPosition(2)*M_PI/180;

    vec3 axe = {1,0,0};


    mat33 yRot = {
        cos(theta), 0, -sin(theta),
        0, 1, 0,
        sin(theta), 0, cos(theta),
    };
    mat33 zRot = {
        cos(psi), sin(psi), 0,
        -sin(psi), cos(psi), 0,
        0,0,1,
    };
    mat33 xRot = {
        1,0,0,
        0, cos(phi), sin(phi),
        0, -sin(phi), cos(phi),
    };


    axe = xRot*yRot*zRot*axe;

    return arma::normalise(axe);
}
arma::vec3 Box::getWorldYAxis() const{

   double phi = _angularPosition(0)*M_PI/180;
    double theta = _angularPosition(1)*M_PI/180;
    double psi = _angularPosition(2)*M_PI/180;

    vec3 axe = {0,1,0};


    mat33 yRot = {
        cos(theta), 0, -sin(theta),
        0, 1, 0,
        sin(theta), 0, cos(theta),
    };
    mat33 zRot = {
        cos(psi), sin(psi), 0,
        -sin(psi), cos(psi), 0,
        0,0,1,
    };
    mat33 xRot = {
        1,0,0,
        0, cos(phi), sin(phi),
        0, -sin(phi), cos(phi),
    };


    axe = xRot*yRot*zRot*axe;

    return arma::normalise(axe);
}
arma::vec3 Box::getWorldZAxis() const{

   double phi = _angularPosition(0)*M_PI/180;
    double theta = _angularPosition(1)*M_PI/180;
    double psi = _angularPosition(2)*M_PI/180;

    vec3 axe = {0,0,1};


    mat33 yRot = {
        cos(theta), 0, -sin(theta),
        0, 1, 0,
        sin(theta), 0, cos(theta),
    };
    mat33 zRot = {
        cos(psi), sin(psi), 0,
        -sin(psi), cos(psi), 0,
        0,0,1,
    };
    mat33 xRot = {
        1,0,0,
        0, cos(phi), sin(phi),
        0, -sin(phi), cos(phi),
    };

    cout << "Axe Z pre-trans " << axe;

    axe = xRot*yRot*zRot*axe;

    cout << "Axe Z ppost-trans " << axe;

    return arma::normalise(axe);
}





//retourne une liste des vertices de la boîte, ordonnés anti-horaire
arma::mat Box::getLocalVertices(){
    double x = this->getDimensionX();
    double y = this->getDimensionY();
    double z = this->getDimensionZ();


    mat listVerts = {
        -x, -y, z,
        x, -y, z,
        x, y , z,
        -x, y, z,
        -x, -y, -z,
        x, -y, -z,
        x, y ,-z,
        -x, y, z
    };
    listVerts.reshape(3, 8);

    /*std::cout << "listeVertices " << x << " "<< y << " " << z << std::endl;
    std::cout << listVerts <<std::endl;*/
    return listVerts;
}
