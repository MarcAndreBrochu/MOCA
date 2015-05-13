#include "Box.h"
#include <exception>
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

    _listVertices = getVertices();
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
    /*mat44 matTrans;
    double phi = _angularPosition(0)*PI/180;
    double theta = _angularPosition(1)*PI/180
    double psi = _angularPosition(2)*PI/180

    matTrans = {
        (cos(theta)*cos(psi)), (cos(phi)*sin(psi)+sin(phi)*sin(theta)*cos(psi)), (sin(phi)*sin(psi)-cos(phi)*sin(theta)*cos(psi)),
        (-cos(theta))
    }*/


    return arma::normalise(_listVertices.col(1) - _listVertices.col(0));
}
arma::vec3 Box::getWorldYAxis() const{
    return arma::normalise(_listVertices.col(3) - _listVertices.col(0) + _position);
}
arma::vec3 Box::getWorldZAxis() const{
    return arma::normalise(_listVertices.col(0) - _listVertices.col(4) + _position)
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
