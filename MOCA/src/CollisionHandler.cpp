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
            //bool colliding = false;
            colliding collide;
            collide.cond = false;

            // indiquent si le type des deux objets de la paire est standard (sphere ou box)
            bool typeA = true;
            bool typeB = true;

            // Code laid qui detecte la nature des deux objets a verifier
            // et qui determine si ils entrent en collision
            if (Ball *ball1 = dynamic_cast<Ball *>(A)) {
                if (Ball *ball2 = dynamic_cast<Ball *>(B))
                    collide.cond = this->detectSS(ball1, ball2);

                else if (Box *box2 = dynamic_cast<Box *>(B))
                    collide.cond = this->detectSB(ball1, box2);

                else typeB = false;
            }
            else if (Box *box1 = dynamic_cast<Box *>(A)) {
                if (Ball *ball2 = dynamic_cast<Ball *>(B))
                    collide.cond = this->detectSB(ball2, box1);

                else if (Box *box2 = dynamic_cast<Box *>(B))
                    collide = this->detectBB(box1, box2);

                else typeB = false;
            }
            else typeA = false;

            // Si on ne peut trouver de methode specifique (car l'un des objets ou les
            // deux objets ne sont pas des spheres ou des boites ou un melange), il faut
            // appeler la methode generique
            if (!typeA || !typeB)
                collide.cond = this->detectGeneric(A, B);

            if (collide.cond) {

                CollisionPair cpair;
                cpair.A = A;
                cpair.B = B;
                cpair.mtv = collide.mtv;

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
                this->resolveBB(box1, box2, it.mtv);

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

colliding CollisionHandler::detectBB(const Box *A, const Box *B) {
    /*Detection de la collision a l'aide du Separating Axis Theorem
        Il y a 15 axes a tester
        Si T.L est plus grand que la condition de droite, alors L est un axe séparateur,
        c'est-à-dire qu'il n'y a pas collision
        */

    colliding collision;
    collision.cond = false;
    collision.mtv = {0,0,0};

    //Définissons ces variables pour faciliter la lecture
    vec3 Pa = A->getPosition();
    vec3 Pb = B->getPosition();
    vec3 T = Pb - Pa;

    mat33 a_axes;
    a_axes.col(0) = A->getWorldXAxis(); a_axes.col(1) = A->getWorldYAxis(); a_axes.col(2) = A->getWorldZAxis();

    mat33 b_axes;
    b_axes.col(0) = B->getWorldXAxis(); b_axes.col(1) = B->getWorldYAxis(); b_axes.col(2) = B->getWorldZAxis();

    vec3 a_dims;
    a_dims << A->getDimensionX() << A->getDimensionY() << A->getDimensionZ();
    vec3 b_dims;
    b_dims << B->getDimensionX() << B->getDimensionY() << B->getDimensionZ();


    mat33 R;
    for(int i = 0;i<3;i++)
        for(int j=0;j<3;j++)
            R(j, i) = dot(a_axes.col(i), b_axes.col(j));

    R = trans(R);

    /*cout << "T       : " << T << endl;
    cout << "Matr R  : " << R << endl;
    cout << "Boéte a : " << a_axes.col(0) << " " << a_axes.col(1) << " " << a_axes.col(2) << endl;
    cout << "Boéte B : " << b_axes.col(0) << " " << b_axes.col(2) << " " << b_axes.col(2) << endl;*/

    vector<colliding> tests(15, collision);

    for(int i=0;i<3;i++){
        if(abs(dot(T, a_axes.col(i))) < a_dims(i) + abs(b_dims(0)*R(i, 0))+ abs(b_dims(1)*R(i,1)) + abs(b_dims(2)*R(i,2))){
            tests[i].cond=true;
            double overlap = a_dims(i) + abs(b_dims(0)*R(i, 0))+ abs(b_dims(1)*R(i,1)) + abs(b_dims(2)*R(i,2)) - abs(dot(T, a_axes.col(i)));
            tests[i].mtv = overlap*a_axes.col(i);
        }
    }
    for(int i=0;i<3;i++){
        if(abs(dot(T, b_axes.col(i))) < b_dims(i) + abs(a_dims(0)*R(0, i))+ abs(a_dims(1)*R(1,i)) + abs(b_dims(2)*R(2,i))){
            tests[i+3].cond=true;
            double overlap = b_dims(i) + abs(a_dims(0)*R(0, i))+ abs(a_dims(1)*R(1,i)) + abs(b_dims(2)*R(2,i)) - abs(dot(T, b_axes.col(i)));
            tests[i+3].mtv = overlap*b_axes.col(i);
        }
    }
    //pour les 9 autres axes, le rapport trouver une boucle qui marche versus recopier les conditions est en défaveur de la boucle
    //ATTENTION : CODE RAVIOLI
    //Ax
    if(abs(dot(T, cross(a_axes.col(0), b_axes.col(0)))) < abs(a_dims(1)*R(2,0)) + abs(a_dims(2)*R(1,0)) + abs(b_dims(1)*R(0,2)) + abs(b_dims(2)*R(0,1))){
        tests[6].cond=true;
        double overlap = abs(a_dims(1)*R(2,0)) + abs(a_dims(2)*R(1,0)) + abs(b_dims(1)*R(0,2)) + abs(b_dims(2)*R(0,1)) - abs(dot(T, cross(a_axes.col(0), b_axes.col(0))));
        tests[6].mtv = cross(a_axes.col(0), b_axes.col(0))*overlap;
    }
    if(abs(dot(T, cross(a_axes.col(0), b_axes.col(1)))) < abs(a_dims(1)*R(2,1)) + abs(a_dims(2)*R(1,1)) + abs(b_dims(0)*R(0,2)) + abs(b_dims(2)*R(0,0))){
        tests[7].cond=true;
        double overlap = abs(a_dims(1)*R(2,1)) + abs(a_dims(2)*R(1,1)) + abs(b_dims(0)*R(0,2)) + abs(b_dims(2)*R(0,0)) - abs(dot(T, cross(a_axes.col(0), b_axes.col(1))));
        tests[7].mtv = cross(a_axes.col(0), b_axes.col(1))*overlap;
    }
    if(abs(dot(T, cross(a_axes.col(0), b_axes.col(2)))) < abs(a_dims(1)*R(2,2)) + abs(a_dims(2)*R(1,2)) + abs(b_dims(0)*R(0,1)) + abs(b_dims(1)*R(0,0))){
        tests[8].cond=true;
        double overlap = abs(a_dims(1)*R(2,2)) + abs(a_dims(2)*R(1,2)) + abs(b_dims(0)*R(0,1)) + abs(b_dims(1)*R(0,0)) - abs(dot(T, cross(a_axes.col(0), b_axes.col(2))));
        tests[8].mtv = cross(a_axes.col(0), b_axes.col(2))*overlap;
    }
    //Ay
    if(abs(dot(T, cross(a_axes.col(1), b_axes.col(0)))) < abs(a_dims(0)*R(2,0)) + abs(a_dims(2)*R(0,0)) + abs(b_dims(1)*R(1,2)) + abs(b_dims(2)*R(1,1))){
        tests[9].cond=true;
        double overlap = abs(a_dims(0)*R(2,0)) + abs(a_dims(2)*R(0,0)) + abs(b_dims(1)*R(1,2)) + abs(b_dims(2)*R(1,1)) - abs(dot(T, cross(a_axes.col(1), b_axes.col(0))));
        tests[9].mtv = cross(a_axes.col(1), b_axes.col(0))*overlap;
    }
    if(abs(dot(T, cross(a_axes.col(1), b_axes.col(1)))) < abs(a_dims(0)*R(2,1)) + abs(a_dims(2)*R(0,1)) + abs(b_dims(0)*R(1,2)) + abs(b_dims(2)*R(1,0))){
        tests[10].cond=true;
        double overlap = abs(a_dims(0)*R(2,1)) + abs(a_dims(2)*R(0,1)) + abs(b_dims(0)*R(1,2)) + abs(b_dims(2)*R(1,0)) - abs(dot(T, cross(a_axes.col(1), b_axes.col(1))));
        tests[10].mtv = cross(a_axes.col(1), b_axes.col(1))*overlap;
    }
    if(abs(dot(T, cross(a_axes.col(1), b_axes.col(2)))) < abs(a_dims(0)*R(2,2)) + abs(a_dims(2)*R(0,2)) + abs(b_dims(0)*R(1,1)) + abs(b_dims(1)*R(1,0))){
        tests[11].cond=true;
        double overlap = abs(a_dims(0)*R(2,2)) + abs(a_dims(2)*R(0,2)) + abs(b_dims(0)*R(1,1)) + abs(b_dims(1)*R(1,0)) - abs(dot(T, cross(a_axes.col(1), b_axes.col(2))));
        tests[11].mtv = cross(a_axes.col(1), b_axes.col(2))*overlap;
    }
    //Az
    if(abs(dot(T, cross(a_axes.col(2), b_axes.col(0)))) < abs(a_dims(0)*R(1,0)) + abs(a_dims(1)*R(0,0)) + abs(b_dims(1)*R(2,2)) + abs(b_dims(2)*R(2,1))){
        tests[12].cond=true;
        double overlap = abs(a_dims(0)*R(1,0)) + abs(a_dims(1)*R(0,0)) + abs(b_dims(1)*R(2,2)) + abs(b_dims(2)*R(2,1)) - abs(dot(T, cross(a_axes.col(2), b_axes.col(0))));
        tests[12].mtv = cross(a_axes.col(2), b_axes.col(0))*overlap;
    }
    if(abs(dot(T, cross(a_axes.col(2), b_axes.col(1)))) < abs(a_dims(0)*R(1,1)) + abs(a_dims(1)*R(0,1)) + abs(b_dims(0)*R(2,2)) + abs(b_dims(2)*R(2,0))){
        tests[13].cond=true;
        double overlap = abs(a_dims(0)*R(1,1)) + abs(a_dims(1)*R(0,1)) + abs(b_dims(0)*R(2,2)) + abs(b_dims(2)*R(2,0)) - abs(dot(T, cross(a_axes.col(2), b_axes.col(1))));
        tests[13].mtv = cross(a_axes.col(2), b_axes.col(1))*overlap;
    }
    if(abs(dot(T, cross(a_axes.col(2), b_axes.col(2)))) < abs(a_dims(1)*R(2,2)) + abs(a_dims(2)*R(1,2)) + abs(b_dims(0)*R(0,1)) + abs(b_dims(1)*R(0,0))){
        tests[14].cond=true;
        double overlap = abs(a_dims(0)*R(1,2)) + abs(a_dims(1)*R(0,2)) + abs(b_dims(0)*R(2,1)) + abs(b_dims(1)*R(2,0)) - abs(dot(T, cross(a_axes.col(2), b_axes.col(2))));
        tests[14].mtv = cross(a_axes.col(2), b_axes.col(2))*overlap;
    }

    collision.cond = true;
    for(auto it : tests){
        if(it.cond == false){
            collision.cond = false;
            return collision;
        }
        else{
            if(norm(collision.mtv) > norm(it.mtv) || norm(collision.mtv) == 0)
                collision.mtv = it.mtv;//On trouve le vecteur le plus petit
        }
    }
    //Si on sort de la boucle, c'est que les tests sur tous les axes etaient vrais et donc qu'il y a collision
    return collision;
/*
    //ATTENTION CODE RAVIOLI
    //Tests de projection des faces de A
    if((abs(dot(T, a_axes.col(0))) < a_dims(0) + abs(b_dims(0)*dot(a_axes.col(0), b_axes.col(0))) + abs(b_dims(1)*dot(a_axes.col(0), b_axes.col(2))) + abs(b_dims(2)*dot(a_axes.col(0), b_axes.col(2)))) &&
        (abs(dot(T, a_axes.col(1))) < a_dims(1) + abs(b_dims(0)*dot(a_axes.col(1), b_axes.col(0))) + abs(b_dims(1)*dot(a_axes.col(1), b_axes.col(2))) + abs(b_dims(2)*dot(a_axes.col(1), b_axes.col(2)))) &&
        (abs(dot(T, a_axes.col(2))) < a_dims(2) + abs(b_dims(0)*dot(a_axes.col(2), b_axes.col(0))) + abs(b_dims(1)*dot(a_axes.col(2), b_axes.col(2))) + abs(b_dims(2)*dot(a_axes.col(2), b_axes.col(2)))) &&
        //Tests de projection des faces de B
        (abs(dot(T, b_axes.col(0))) < b_dims(0) + abs(a_dims(0)*dot(a_axes.col(0), b_axes.col(0))) + abs(a_dims(1)*dot(a_axes.col(1), b_axes.col(0))) + abs(a_dims(2)*dot(a_axes.col(2), b_axes.col(0)))) &&
        (abs(dot(T, b_axes.col(1))) < b_dims(1) + abs(a_dims(0)*dot(a_axes.col(0), b_axes.col(1))) + abs(a_dims(1)*dot(a_axes.col(1), b_axes.col(1))) + abs(a_dims(2)*dot(a_axes.col(2), b_axes.col(2)))) &&
        (abs(dot(T, b_axes.col(2))) < b_dims(2) + abs(a_dims(0)*dot(a_axes.col(0), b_axes.col(2))) + abs(a_dims(1)*dot(a_axes.col(1), b_axes.col(2))) + abs(a_dims(2)*dot(a_axes.col(2), b_axes.col(2)))) &&
        //Tests des aretes
        //Ax
        (abs(dot(T, cross(a_axes.col(0), b_axes.col(0)))) < abs(a_dims(1)*R(2,0)) + abs(a_dims(2)*R(1,0)) + abs(b_dims(1)*R(0,2)) + abs(b_dims(2)*R(0,1))) &&
        (abs(dot(T, cross(a_axes.col(0), b_axes.col(1)))) < abs(a_dims(1)*R(2,1)) + abs(a_dims(2)*R(1,1)) + abs(b_dims(0)*R(0,2)) + abs(b_dims(2)*R(0,0))) &&
        (abs(dot(T, cross(a_axes.col(0), b_axes.col(2)))) < abs(a_dims(1)*R(2,2)) + abs(a_dims(2)*R(1,2)) + abs(b_dims(0)*R(0,1)) + abs(b_dims(1)*R(0,0))) &&
        //Ay
        (abs(dot(T, cross(a_axes.col(1), b_axes.col(0)))) < abs(a_dims(0)*R(2,0)) + abs(a_dims(2)*R(0,0)) + abs(b_dims(1)*R(1,2)) + abs(b_dims(2)*R(1,1))) &&
        (abs(dot(T, cross(a_axes.col(1), b_axes.col(1)))) < abs(a_dims(0)*R(2,1)) + abs(a_dims(2)*R(0,1)) + abs(b_dims(0)*R(1,2)) + abs(b_dims(2)*R(1,0))) &&
        (abs(dot(T, cross(a_axes.col(1), b_axes.col(2)))) < abs(a_dims(0)*R(2,2)) + abs(a_dims(2)*R(0,2)) + abs(b_dims(0)*R(1,1)) + abs(b_dims(1)*R(1,0))) &&
        //Az
        (abs(dot(T, cross(a_axes.col(2), b_axes.col(0)))) < abs(a_dims(0)*R(1,0)) + abs(a_dims(1)*R(0,0)) + abs(b_dims(1)*R(2,2)) + abs(b_dims(2)*R(2,1))) &&
        (abs(dot(T, cross(a_axes.col(2), b_axes.col(1)))) < abs(a_dims(0)*R(1,1)) + abs(a_dims(1)*R(0,1)) + abs(b_dims(0)*R(2,2)) + abs(b_dims(2)*R(2,0))) &&
        (abs(dot(T, cross(a_axes.col(2), b_axes.col(2)))) < abs(a_dims(0)*R(1,2)) + abs(a_dims(1)*R(0,2)) + abs(b_dims(0)*R(2,1)) + abs(b_dims(1)*R(2,0))))
        return true;
    //Si chaque condition est fausse, alors il y a collision
    else
        return false;*/

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

void CollisionHandler::resolveBB(Box *A, Box *B, vec3 mtv) {

    cout << "Le MTV : " << endl << mtv << endl;

    vec3 iniPosA = A->getPosition();
    vec3 iniPosB = B->getPosition();
    //vec3 finalPosA = iniPosA - mtv;
    vec3 finalPosB = iniPosB + mtv;


    //B->setPosition(finalPosB);

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
