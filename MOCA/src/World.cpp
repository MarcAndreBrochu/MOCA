#include <exception>
#include "World.h"

using namespace std;
using namespace arma;

World::World() {
    _lastKeyAssignedBody = -1; // -1 sur un uint => uint::max
    _lastKeyAssignedForce = -1;
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

uint World::applyForce(const vec3 &force) {
    _forces[++_lastKeyAssignedForce] = force;
    return _lastKeyAssignedForce;
}

void World::removeForce(uint fid) {

    // On doit s'assurer que la force existe dans le monde
    unordered_map<uint, vec3>::const_iterator item = _forces.find(fid);
    if (item == _forces.end())
        throw out_of_range("Cannot remove: force doesn't exists");

    _forces.erase(item);
}

void World::updateWorld(){
  for(auto i = _bodies.begin(); i != _bodies.end(); i++)
  {
    _bodies[i]->updateBody(_timeStep);
  }

}
