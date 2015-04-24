#ifndef MOCA_COLLISION_HANDLER_H
#define MOCA_COLLISION_HANDLER_H

#include <vector>

class AbstractBody;

/**
 * @class CollisionHandler
 * @brief Gere les collisions entre les corps.
 */
class CollisionHandler {

public:
    CollisionHandler();
    ~CollisionHandler();

    static CollisionHandler *sharedInstance();

    struct CollisionPair {
        AbstractBody *A;
        AbstractBody *B;
    };

    std::vector<CollisionPair> detectCollisions();

private:
    static CollisionHandler *_singleton;
};

#endif // MOCA_COLLISION_HANDLER_H
