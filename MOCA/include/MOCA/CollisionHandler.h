#ifndef MOCA_COLLISION_HANDLER_H
#define MOCA_COLLISION_HANDLER_H

#include <vector>
#include <unordered_map>
#include <armadillo>

class AbstractBody;
class Ball;
class Box;

typedef unsigned int uint;
using namespace arma;

//Une petite structurette pour gerer les resultats sortant de la detection de collision qui ne sont pas que des booles
struct colliding {
    bool cond;
    vec3 mtv = {0, 0, 0};
};


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
        //Minimum Translation Vector - déplacement minimum pour que les objets ne s'intersectent plus
        vec3 mtv = {0, 0, 0};
    };

    /**
     * Detecte les collisions entre les objets d'un pool sachant que les objets ne peuvent etre que des spheres
     * ou des prismes a base rectangulaire.
     * @param pool La liste des objets a tester
     * @return La liste de toutes les collisions. Si A est en collision avec B et C, retourner A-B et A-C
     */
    std::vector<CollisionPair> detectCollisions(const std::unordered_map<uint, AbstractBody *> &pool);

    /**
     * Resoud (applique les impulsions necessaires) sur les objets en collision.
     * @param pool La liste des objets en collision
     */
    void resolveCollisions(std::vector<CollisionPair> &pool);

private:
    bool detectSS(const Ball *A, const Ball *B); // collision sphere-sphere
    colliding detectBB(const Box *A, const Box *B);   // collision box-box
    bool detectSB(const Ball *A, const Box *B);  // collision sphere-box
    bool detectGeneric(const AbstractBody *A, const AbstractBody *B); // collision generique

    void resolveSS(Ball *A, Ball *B);
    void resolveSB(Ball *A, Box *B);
    void resolveBB(Box *A, Box *B, vec3 mtv);
    void resolveGeneric(AbstractBody *A, AbstractBody *B);

    static CollisionHandler *_singleton;

};

#endif // MOCA_COLLISION_HANDLER_H
