#include <exception>
#include "AbstractBody.h"

using namespace std;
using namespace arma;

AbstractBody::AbstractBody() {
    _lastKeyAssigned = -1; // meme si c'est un uint, uint::max + 1 = 0
    _isFixed = false;

    _mass = 1; // une masse de 1 par defaut pour eviter les probs de div par zero
    _position = {0, 0, 0};
    _velocity = {0, 0, 0};
}
AbstractBody::~AbstractBody() {}

void AbstractBody::applyImpulse(const vec3 &imp) {
    _impulse += imp;
}

uint AbstractBody::applyForce(const arma::vec3 &force) {

    // On cree une nouvelle entree dans la map des forces avec comme key
    // le nouvel index
    _forces[++_lastKeyAssigned] = vec3(force);
    //On additionne egalement la force a la somme de forces
    _sumForces += force;
    return _lastKeyAssigned;
}

void AbstractBody::removeForce(uint fid) {

    // Il faut s'assurer qu'on va enlever une force qui existe
    unordered_map<uint, vec3>::const_iterator emp = _forces.find(fid);
    if (emp == _forces.end())
        throw out_of_range("Cannot remove: force doesn't exists");

    _sumForces -= emp->second;
    _forces.erase(emp);
}

vec3 AbstractBody::getAcceleration() const {
    return _sumForces / _mass;
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
