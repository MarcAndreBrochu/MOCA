#include <exception>
#include "World.h"
#include "AbstractBody.h"
#include "Integrator.h"
#include "Utils.h"

using namespace std;
using namespace arma;

World::World() {

    _lastKeyAssignedBody = -1; // -1 sur un uint => uint::max
    _lastKeyAssignedForce = -1;

    _sumAccel = zeros<vec>(3);
}
World::~World() {}

uint World::addBody(AbstractBody *body) {
    _bodies[++_lastKeyAssignedBody] = body;
    return _lastKeyAssignedBody;
}

void World::removeBody(AbstractBody *body) {

    // Il s'agit de trouver l'index du corps en premier
    uint bid = -1;
    for (auto it : _bodies) {
        if (it.second == body)
            bid = it.first;
    }
    this->removeBody(bid);
}

void World::removeBody(uint bid) {

    // On doit s'assurer que le corps existe dans le monde
    unordered_map<uint, AbstractBody *>::const_iterator item = _bodies.find(bid);
    if (item == _bodies.end())
        throw out_of_range("Cannot remove: body doesn't exists");

    _bodies.erase(item);
}

uint World::applyAcceleration(const vec3 &accel) {

    _accels[++_lastKeyAssignedForce] = accel;
    _sumAccel += accel;
    return _lastKeyAssignedForce;
}

void World::removeAcceleration(uint fid) {

    // On doit s'assurer que la force existe dans le monde
    unordered_map<uint, vec3>::const_iterator item = _accels.find(fid);
    if (item == _accels.end())
        throw out_of_range("Cannot remove: force doesn't exists");

    _accels.erase(item);
    _sumAccel -= item->second;
}

void World::updateWorld(double timeStep) {

    for (auto i = _bodies.begin(); i != _bodies.end(); i++) {

        AbstractBody *body = i->second;

        this->updateBody(body, timeStep);
        body->resetImpulsion(); // l'impulsion est reset a chaque frame, par definition
    }
}

void World::updateBody(AbstractBody *body, double timeStep) {

    vec3 pos = body->getPosition();
    vec3 velocity = body->getVelocity();
    vec3 impulse = body->getImpulsion();
    vec3 accel = body->getAcceleration();

    //on change la velocite selon les impulsions si elle ne sont pas nulles
    if (any(impulse)) {

        double mass = body->getMass();

        // Theoreme de l'impulsion:
        //      J = mv_2 - mv_1
        //   => v_2 = (J + mv_1) / m
        velocity[0] = (velocity[0] * mass + impulse[0]) / mass;
        velocity[1] = (velocity[1] * mass + impulse[1]) / mass;
        velocity[2] = (velocity[2] * mass + impulse[2]) / mass;

        // Comme l'accel pendant l'impulsion est constante, la pente de
        // la velocite va l'etre aussi. Ainsi, x = v*t
        pos += velocity * timeStep;
    }

    // Il s'agit maintenant d'integrer sur les accelerations afin de trouver une
    // nouvelle velocite qui va s'ajouter a celles trouvees ci-haut avec les impulsions.
    // Le if est la pour empecher les erreurs d'arrondi (cf IEEE)
    if (MOCA_GREATER_THAN_ZERO(norm(accel))) {
        Integrator::Status ns = Integrator::verlet(pos, velocity, accel, accel, timeStep);
        velocity = ns.v;
        pos = ns.p;
    }

    // Il faut maintenant processer l'acceleration globale sur l'objet si l'accel
    // est plus grande que 0
    if (MOCA_GREATER_THAN_ZERO(norm(_sumAccel))) {
        Integrator::Status ns = Integrator::verlet(pos, velocity, _sumAccel, _sumAccel, timeStep);
        velocity = ns.v;
        pos = ns.p;
    }

    body->setVelocity(velocity);
    body->setPosition(pos);
}
