#ifndef MOCA_PARTICLE_H
#define MOCA_PARTICLE_H

#include "AbstractBody.h"

/**
 * @class Particle
 * @brief Un corps rigide qui ne possede pas de volume.
 *
 * Une particule est definie comme etant un corps dont toute la masse est concentree
 * en un seul point, sa position.
 */
class Particle : public AbstractBody {

public:
    Particle();
    virtual ~Particle();

private:
};

#endif // MOCA_PARTICLE_H
