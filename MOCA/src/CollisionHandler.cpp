#include "CollisionHandler.h"

CollisionHandler *CollisionHandler::_singleton = nullptr;
CollisionHandler *CollisionHandler::sharedInstance() {
    if (!_singleton)
        _singleton = new CollisionHandler;
    return _singleton;
}

CollisionHandler::CollisionHandler() {}
CollisionHandler::~CollisionHandler() {}
