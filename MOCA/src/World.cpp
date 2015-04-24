#include <exception>
#include "World.h"
#include "AbstractBody.h"
#include "Solid.h"
#include "Integrator.h"
#include "Utils.h"
#include "CollisionHandler.h"

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

    _sumAccel -= item->second;
    _accels.erase(item);
}

void World::updateWorld(double timeStep) {

    for (auto i = _bodies.begin(); i != _bodies.end(); i++) {

        AbstractBody *body = i->second;

        // Si le corps est fixe, il n'est pas affecte par les forces et les impulsions
        if (!(i->second->isFixed())) {
            this->updateBody(body, timeStep);
            body->resetImpulse(); // l'impulsion est reset a chaque frame, par definition
        }
    }

    CollisionHandler *chandler = CollisionHandler::sharedInstance();
}

void World::updateBody(AbstractBody *body, double timeStep) {

    vec3 pos = body->getPosition();
    vec3 velocity = body->getVelocity();
    vec3 impulse = body->getImpulse();
    vec3 accel = body->getAcceleration();

    // .............................................................................
    // Mouvement recktiligne
    // .............................................................................
    //on change la velocite selon les impulsions
    double mass = body->getMass();

    // Theoreme de l'impulsion:
    //      J = mv_2 - mv_1
    //   => v_2 = (J + mv_1) / m
    velocity = (velocity * mass + impulse) / mass;
    pos += velocity * timeStep;

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

    // .............................................................................
    // Mouvement de rotation
    // .............................................................................
    // Seuls les solides peuvent avoir une rotation, donc on exit la fonction si
    // c'est une particule
    Solid *solidBody = dynamic_cast<Solid *>(body);
    if (!solidBody)
        return;

    // Il faut savoir que la methode ne va pas changer l'acceleration angulaire. C'est lors
    // des collisions qu'un tel phenomene se produit. Pourtant, il faut quand meme updater
    // la rotation de l'objet, au cas ou il aurait une vitesse angulaire non-nulle ou une
    // acceleration angulaire non-nulle (ce dernier cas n'etant possible que si il y a eu
    // collision avant l'appel de cette methode).
    vec3 angulPos = solidBody->getAngularPosition();
    vec3 angulVel = solidBody->getAngularVelocity();
    vec3 angulImp = solidBody->getAngularImpulse();

    mat33 I = solidBody->getInertia(); // l'equivalent de la masse mais en rotation

    // Si on adapte le theoreme de l'impulsion au mouvement rotationnel, ca pourrait donner
    // qqchose comme ca:
    // Jrot = ∆L
    // Jrot = I*w_2 - I*w_1
    // w_2 = (Jrot + I*w_1)/I
    vec3 topOp = angulImp + I * angulVel;
    angulVel = inv(diagmat(I)) * topOp;

    angulPos += angulVel * timeStep;

    solidBody->setAngularVelocity(angulVel);
    solidBody->setAngularPosition(angulPos);
}
