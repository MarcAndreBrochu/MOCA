#include "CollisionHandler.h"
#include "AbstractBody.h"
#include "Ball.h"
#include "Box.h"

#include <cassert>
#include <iostream>

using namespace std;

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

bool CollisionHandler::detectSS(const Ball *A, const Ball *B) {}
bool CollisionHandler::detectBB(const Box *A, const Box *B) {}
bool CollisionHandler::detectSB(const Ball *A, const Box *B) {}

bool CollisionHandler::detectGeneric(const AbstractBody *A, const AbstractBody *B) {
    cerr << "SAT collision detection not implemented yet" << endl;
    assert(false && A != B); // 'A != B' seulement la pour empecher les unused var warnings, sert a rien
    return false;
}
