#include "CollisionHandler.h"
#include "AbstractBody.h"
#include "Ball.h"
#include "Box.h"

#include <cassert>
#include <iostream>

#include <armadillo>

using namespace std;
using namespace arma;

CollisionHandler *CollisionHandler::_singleton = nullptr;
CollisionHandler *CollisionHandler::sharedInstance() {
    if (!_singleton)
        _singleton = new CollisionHandler;
    return _singleton;
}

CollisionHandler::CollisionHandler() {}
CollisionHandler::~CollisionHandler() {
    if (_singleton) {
        delete _singleton;
        _singleton = nullptr;
    }
}

vector<CollisionHandler::CollisionPair> CollisionHandler::detectCollisions(const unordered_map<uint, AbstractBody *> &pool) {

    vector<CollisionPair> collList;

    // Si le world ne contient qu'un corps ou est vide, alors c'est evident qu'il n'y a pas de
    // collisions. On retourne alors la liste vide
    if (pool.size() <= 1)
        return collList;

    // Le but est de tester toutes les paires d'objets possibles en sachant que A-B == B-A et donc
    // en evitant cette situation. Comment faire?
    //  1) Prendre le premier element
    //  2) Tester cet element avec tous les autres
    //  3) Prendre le second element et le tester avec tous les autres sauf le premier
    //  4) etc.
    // De facon plus symbolique:
    //  1) i=0
    //  2) tester(obj(#=i), obj(#>i))
    //  3) i++, goto 2)

    for (uint i = 0; i < pool.size(); i++) {

        AbstractBody *A = pool.at(i);
        for (uint j = i + 1; j < pool.size(); j++) {

            AbstractBody *B = pool.at(j);
            bool colliding = false;
            // indiquent si le type des deux objets de la paire est standard (sphere ou box)
            bool typeA = true;
            bool typeB = true;

            // Code laid qui detecte la nature des deux objets a verifier
            // et qui determine si ils entrent en collision
            if (Ball *ball1 = dynamic_cast<Ball *>(A)) {
                if (Ball *ball2 = dynamic_cast<Ball *>(B))
                    colliding = this->detectSS(ball1, ball2);

                else if (Box *box2 = dynamic_cast<Box *>(B))
                    colliding = this->detectSB(ball1, box2);

                else typeB = false;
            }
            else if (Box *box1 = dynamic_cast<Box *>(A)) {
                if (Ball *ball2 = dynamic_cast<Ball *>(B))
                    colliding = this->detectSB(ball2, box1);

                else if (Box *box2 = dynamic_cast<Box *>(B))
                    colliding = this->detectBB(box1, box2);

                else typeB = false;
            }
            else typeA = false;

            // Si on ne peut trouver de methode specifique (car l'un des objets ou les
            // deux objets ne sont pas des spheres ou des boites ou un melange), il faut
            // appeler la methode generique
            if (!typeA || !typeB)
                colliding = this->detectGeneric(A, B);

            if (colliding) {

                CollisionPair cpair;
                cpair.A = A;
                cpair.B = B;

                collList.push_back(cpair);
            }
        }
    }

    return collList;
}

void CollisionHandler::resolveCollisions(std::vector<CollisionPair> &pool) {

    for (auto it : pool) {

        AbstractBody *A = it.A;
        AbstractBody *B = it.B;
        // indiquent si le type des deux objets de la paire est standard (sphere ou box)
        bool typeA = true;
        bool typeB = true;

        if (Ball *ball1 = dynamic_cast<Ball *>(A)) {
            if (Ball *ball2 = dynamic_cast<Ball *>(B))
                this->resolveSS(ball1, ball2);

            else if (Box *box2 = dynamic_cast<Box *>(B))
                this->resolveSB(ball1, box2);

            else typeB = false;
        }
        else if (Box *box1 = dynamic_cast<Box *>(A)) {
            if (Ball *ball2 = dynamic_cast<Ball *>(B))
                this->resolveSB(ball2, box1);

            else if (Box *box2 = dynamic_cast<Box *>(B))
                this->resolveBB(box1, box2);

            else typeB = false;
        }
        else typeA = false;

        // Si on ne peut trouver de methode specifique (car l'un des objets ou les
        // deux objets ne sont pas des spheres ou des boites ou un melange), il faut
        // appeler la methode generique
        if (!typeA || !typeB)
            this->resolveGeneric(A, B);
    }
}

// Detection de collision
bool CollisionHandler::detectSS(const Ball *A, const Ball *B) {

    double radiusA = A->getRadius();
    double radiusB = B->getRadius();
    double distance = norm(A->getPosition() - B->getPosition());

    if (distance < radiusA + radiusB)
        return true;

    return false;
}

bool CollisionHandler::detectBB(const Box *A, const Box *B) {}
bool CollisionHandler::detectSB(const Ball *A, const Box *B) {}

bool CollisionHandler::detectGeneric(const AbstractBody *A, const AbstractBody *B) {
    cerr << "SAT collision detection not implemented yet" << endl;
    assert(false && A != B); // 'A != B' seulement la pour empecher les unused var warnings, sert a rien
    return false;
}

// Resolution de collisions
void CollisionHandler::resolveSS(Ball *A, Ball *B) {

    // Les vitesses resultantes d'une collision inelastique sur une dimension
    // sont donnees par:
    // vf_a = (Cr*m_b*(vi_b - vi_a) + m_a*vi_a + m_b*vi_b) / (m_a + m_b)
    // vf_b = (Cr*m_a*(vi_a - vi_b) + m_a*vi_a + m_b*vi_b) / (m_a + m_b)
    // [Cr est le coefficient de restitution (=sqrt(h2/h1))]
    //
    // "Pour utiliser cette formule sur plusieurs dimensions, il suffit de prendre les
    // composantes perpendiculaires tangentes à la droite ou plan du point de contact."

    double restitution = 0.7;
    double massA = A->getMass();
    double massB = B->getMass();

    vec3 initialVelA = A->getVelocity();
    vec3 initialVelB = B->getVelocity();

    // Il faut maintenant obtenir la vitesse par-rapport au plan tangeant au point de
    // contact... Si on a un vecteur normal a ce plan, on peut simplement projeter le
    // vecteur vitesse sur ce vecteur normal afin d'obtenir un nouveau vecteur vitesse.
    vec3 surfaceNormal = B->getPosition() - A->getPosition();
    initialVelA = (dot(initialVelA, surfaceNormal) / (norm(surfaceNormal) * norm(surfaceNormal))) * surfaceNormal;
    initialVelB = (dot(initialVelB, -surfaceNormal) / (norm(-surfaceNormal) * norm(-surfaceNormal))) * -surfaceNormal;

    vec3 constantTop = massA * initialVelA + massB * initialVelB; // La partie du numerateur qui reste constante
    vec3 finalVelA  = (restitution * massB * (initialVelB - initialVelA) + constantTop) / (massA + massB);
    vec3 finalVelB  = (restitution * massA * (initialVelA - initialVelB) + constantTop) / (massA + massB);

    vec3 impulseA = massA * (finalVelA - initialVelA);
    vec3 impulseB = massB * (finalVelB - initialVelB);
    A->applyImpulse(impulseA);
    B->applyImpulse(impulseB);
}

void CollisionHandler::resolveSB(Ball *A, Box *B) {}
void CollisionHandler::resolveBB(Box *A, Box *B) {}
void CollisionHandler::resolveGeneric(AbstractBody *A, AbstractBody *B) {}
