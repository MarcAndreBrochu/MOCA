#include "World.h"

World::World() {
    _lastKeyAssigned = -1; // -1 sur un uint => uint::max
            //Pourquoi pas juste le mettre a 0 ? -LBL

    _timeStep = 1/60*1000//On initialise le step par défaut à 60fps

}
World::~World() {}

uint World::addBody(AbstractBody *body) {
    _bodies[++_lastKeyAssigned] = body;
    return _lastKeyAssigned;
}

void World::updateWorld(){
  for(auto i = _bodies.begin(); i != _bodies.end(); i++)
  {
    _bodies[i]->updateBody(_timeStep);
  }

}
