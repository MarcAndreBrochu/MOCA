#ifndef MOCA_COLLISION_HANDLER_H
#define MOCA_COLLISION_HANDLER_H

#include <vector>
#include <unordered_map>

class AbstractBody;
class Ball;
class Box;

typedef unsigned int uint;

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

    /**
     * Detecte les collisions entre les objets d'un pool sachant que les objets ne peuvent etre que des spheres
     * ou des prismes a base rectangulaire.
     * @return La liste de toutes les collisions. Si A est en collision avec B et C, retourner A-B et A-C
     */
    std::vector<CollisionPair> detectCollisions(const std::unordered_map<uint, AbstractBody *> &pool);

private:
    bool detectSS(const Ball *A, const Ball *B); // collision sphere-sphere
    bool detectBB(const Box *A, const Box *B);   // collision box-box
    bool detectSB(const Ball *A, const Box *B);  // collision sphere-box
    bool detectGeneric(const AbstractBody *A, const AbstractBody *B); // collision generique

    static CollisionHandler *_singleton;
};

#endif // MOCA_COLLISION_HANDLER_H
