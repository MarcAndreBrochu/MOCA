#include <exception>
#include "AbstractBody.h"

using namespace std;
using namespace arma;

AbstractBody::AbstractBody() {
    _lastKeyAssigned = -1; // meme si c'est un uint, uint::max + 1 = 0
}
AbstractBody::~AbstractBody() {}

void AbstractBody::applyImpulse(const vec3 &imp) {
    _impulsions.push_back(vec3(imp));
}

uint AbstractBody::applyForce(const arma::vec3 &force) {

    // On cree une nouvelle entree dans la map des forces avec comme key
    // le nouvel index
    _forces[++_lastKeyAssigned] = vec3(force);
    //On additionne egalement la force a la somme de forces
    _sumForces = _sumForces + force;
    return _lastKeyAssigned;
}

void AbstractBody::removeForce(uint fid) {

    // Il faut s'assurer qu'on va enlever une force qui existe
    unordered_map<uint, vec3>::const_iterator emp = _forces.find(fid);
    if (emp == _forces.end())
        throw out_of_range("Cannot remove: force doesn't exists");

    _forces.erase(emp);
}

void AbstractBody::updateBody(double timeStep) {
  //on change la velocite selon les impulsions
  this.setVelocity()
  //l'integration "velocity verlet" s'occupe du mouvement rectiligne
  Integrator::verlet(this, timeStep);
  /*mouvement angulaire*/
}

void AbstractBody::uptdSumForces(){
  _sumForces = vec(3, fill::zeros);
  for(auto it = _forces.begin(); it != _forces.end(); it++){
    _sumForces = _sumForces + _forces[it];
  }
}
void AbstractBody::uptdSumImpuls(){
  _sumImpuls = vec(3, fill::zeros);
  for(auto it = _impulsions.begin(); it != _impulsions.end(); it++){
    _sumImpuls = _sumImpuls + _impulsions[it];
  }
}


arma::vec3 AbstractBody::getAcceleration(){
    arma::vec3 a;
    a[0] = _sumForces[0]/_mass;//acceleration en x
    a[1] = _sumForces[1]/_mass;//acceleration en y
    a[2] = _sumForces[2]/_mass;//acceleration en z
    return a;
}

void AbstractBody::setPosition(double x, double y, double z) {
    _position[0] = x;
    _position[1] = y;
    _position[2] = z;
}

void AbstractBody::setVelocity(double vx, double vy, double vz) {
    _velocity[0] = vx;
    _velocity[1] = vy;
    _velocity[2] = vz;
}
