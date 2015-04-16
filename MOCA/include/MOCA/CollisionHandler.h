#ifndef MOCA_COLLISION_HANDLER_H
#define MOCA_COLLISION_HANDLER_H

/**
 * @class CollisionHandler
 * @brief Gere les collisions entre les corps.
 */
class CollisionHandler {

public:
    CollisionHandler();
    ~CollisionHandler();

    static CollisionHandler *sharedInstance();

private:
    static CollisionHandler *_singleton;
};

#endif // MOCA_COLLISION_HANDLER_H
