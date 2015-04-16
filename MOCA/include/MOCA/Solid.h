#ifndef MOCA_SOLID_H
#define MOCA_SOLID_H

#include "AbstractBody.h"

/**
 * @class Solid
 * @brief Un solide est un corps qui possede un volume.
 *
 * Un solide est un corps possedant un volume, et donc un potentiel de rotation.
 * C'est donc plus complique a implementer qu'une simple particule.
 */
class Solid : public AbstractBody {

public:
    Solid();
    virtual ~Solid();

    // Getters et setters
    virtual double getInertia() const = 0;
    virtual double getVolume() const = 0;
};

#endif // MOCA_SOLID_H
