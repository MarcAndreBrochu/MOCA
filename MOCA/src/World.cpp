#include "World.h"

World::World() {
    _lastKeyAssigned = -1; // -1 sur un uint => uint::max
}
World::~World() {}

uint World::addBody(AbstractBody *body) {
    _bodies[++_lastKeyAssigned] = body;
    return _lastKeyAssigned;
}
