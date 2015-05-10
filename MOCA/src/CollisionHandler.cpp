#include "CollisionHandler.h"
#include "AbstractBody.h"
#include "Ball.h"
#include "Box.h"

#include <cassert>
#include <iostream>

#include <armadillo>
#include <cmath>

#include <iostream>

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

bool CollisionHandler::detectBB(const Box *A, const Box *B) {
    /*Detection de la collision a l'aide du Separating Axis Theorem
        Il y a 15 axes a tester
        Si T.L est plus grand que la condition de droite, alors L est un axe séparateur,
        c'est-à-dire qu'il n'y a pas collision
        */

    //Définissons ces variables pour faciliter la lecture
    vec3 Pa = A->getPosition();
    vec3 Pb = B->getPosition();
    vec3 T = Pb - Pa;

    vec3 Ax = A->getXAxis();
    vec3 Ay = A->getYAxis();
    vec3 Az = A->getZAxis();
    mat33 a_axes;
    a_axes.col(0) = A->getXAxis(); a_axes.col(1) = A->getYAxis(); a_axes.col(2) = A->getZAxis();
    vec3 Bx = B->getXAxis();
    vec3 By = B->getYAxis();
    vec3 Bz = B->getZAxis();
    mat33 b_axes;
    b_axes.col(0) = B->getXAxis(); b_axes.col(1) = B->getYAxis(); b_axes.col(2) = B->getZAxis();


    double Wa = A->getDimensionX();
    double Ha = A->getDimensionY();
    double Da = A->getDimensionZ();
    double Wb = B->getDimensionX();
    double Hb = B->getDimensionY();
    double Db = B->getDimensionZ();

    mat33 R;
    for(int i = 0;i<3;i++)
        for(int j=0;j<3;j++)
            R(j, i) = dot(a_axes.col(i), b_axes.col(j));

    R = trans(R);

    cout << "T       : " << T << endl;
    cout << "Matr R  : " << R << endl;
    cout << "Boéte a : " << Ax << " " << Ay << " " << Az << endl;
    cout << "Boéte B : " << Bx << " " << By << " " << Bz << endl;

    //ATTENTION CODE SPAGHETTI
    //Tests de projection des faces de A
    if((abs(dot(T, Ax)) < Wa + abs(Wb*dot(Ax, Bx)) + abs(Hb*dot(Ax, By)) + abs(Db*dot(Ax, Bz))) &&
        (abs(dot(T, Ay)) < Ha + abs(Wb*dot(Ay, Bx)) + abs(Hb*dot(Ay, By)) + abs(Db*dot(Ay, Bz))) &&
        (abs(dot(T, Az)) < Da + abs(Wb*dot(Az, Bx)) + abs(Hb*dot(Az, By)) + abs(Db*dot(Az, Bz))) &&
        //Tests de projection des faces de B
        (abs(dot(T, Bx)) < Wb + abs(Wa*dot(Ax, Bx)) + abs(Ha*dot(Ay, Bx)) + abs(Da*dot(Az, Bx))) &&
        (abs(dot(T, By)) < Hb + abs(Wa*dot(Ax, By)) + abs(Ha*dot(Ay, By)) + abs(Da*dot(Az, By))) &&
        (abs(dot(T, Bz)) < Db + abs(Wa*dot(Ax, Bz)) + abs(Ha*dot(Ay, Bz)) + abs(Da*dot(Az, Bz))) &&
        //Tests des aretes
        //Ax
        (abs(dot(T, cross(Ax, Bx))) < abs(Ha*R(2,0)) + abs(Da*R(1,0)) + abs(Hb*R(0,2)) + abs(Db*R(0,1))) &&
        (abs(dot(T, cross(Ax, By))) < abs(Ha*R(2,1)) + abs(Da*R(1,1)) + abs(Wb*R(0,2)) + abs(Db*R(0,0))) &&
        (abs(dot(T, cross(Ax, Bz))) < abs(Ha*R(2,2)) + abs(Da*R(1,2)) + abs(Wb*R(0,1)) + abs(Hb*R(0,0))) &&
        //Ay
        (abs(dot(T, cross(Ay, Bx))) < abs(Wa*R(2,0)) + abs(Da*R(0,0)) + abs(Hb*R(1,2)) + abs(Db*R(1,1))) &&
        (abs(dot(T, cross(Ay, By))) < abs(Wa*R(2,1)) + abs(Da*R(0,1)) + abs(Wb*R(1,2)) + abs(Db*R(1,0))) &&
        (abs(dot(T, cross(Ay, Bz))) < abs(Wa*R(2,2)) + abs(Da*R(0,2)) + abs(Wb*R(1,1)) + abs(Hb*R(1,0))) &&
        //Az
        (abs(dot(T, cross(Az, Bx))) < abs(Wa*R(1,0)) + abs(Ha*R(0,0)) + abs(Hb*R(2,2)) + abs(Db*R(2,1))) &&
        (abs(dot(T, cross(Az, By))) < abs(Wa*R(1,1)) + abs(Ha*R(0,1)) + abs(Wb*R(2,2)) + abs(Db*R(2,0))) &&
        (abs(dot(T, cross(Az, Bz))) < abs(Wa*R(1,2)) + abs(Ha*R(0,2)) + abs(Wb*R(2,1)) + abs(Hb*R(2,0))))
        return true;
    //Si chaque condition est fausse, alors il y a collision
    else
        return false;

}
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
void CollisionHandler::resolveBB(Box *A, Box *B) {

//COPIE DE RESOLVESS
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
void CollisionHandler::resolveGeneric(AbstractBody *A, AbstractBody *B) {}
