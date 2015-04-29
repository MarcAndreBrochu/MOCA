#include "Solid.h"

using namespace arma;
using namespace std;

Solid::Solid(bool isHollow) {
    _lastKeyAssigned = -1;
    _isHollow = isHollow;

    _angularPosition = {0, 0, 0};
    _angularVelocity = {0, 0, 0};
}
Solid::~Solid() {}

void Solid::applyAngularImpulse(const vec3 aimp) {
    _angularImpulse += aimp;
}

uint Solid::applyTorque(const vec3 torque) {

    // On cree une nouvelle entree dans la map des torques avec comme key
    // le nouvel index
    _torques[++_lastKeyAssigned] = vec3(torque);
    //On additionne egalement la force a la somme de torques
    _sumTorques += torque;
    return _lastKeyAssigned;
}

void Solid::removeTorque(uint tid) {

    // Il faut s'assurer qu'on va enlever une torque qui existe
    unordered_map<uint, vec3>::const_iterator emp = _torques.find(tid);
    if (emp == _torques.end())
        throw out_of_range("Cannot remove: torque doesn't exists");

    _sumTorques -= emp->second;
    _torques.erase(emp);
}

void Solid::resetImpulse() {
    _angularImpulse.zeros();
    AbstractBody::resetImpulse();
}

vec3 Solid::getAngularAcceleration() const {

    // tau = [I] * alpha
    // [I]-1 * tau = [I]-1 * [I] * alpha
    // alpha = [I]-1 * tau
    mat33 I = this->getInertia();
    return inv(diagmat(I)) * _sumTorques;
}

void Solid::setAngularPosition(double tx, double ty, double tz) {
    _angularPosition[0] = tx;
    _angularPosition[1] = ty;
    _angularPosition[2] = tz;
}

void Solid::setAngularVelocity(double wx, double wy, double wz) {
    _angularVelocity[0] = wx;
    _angularVelocity[1] = wy;
    _angularVelocity[2] = wz;
}
