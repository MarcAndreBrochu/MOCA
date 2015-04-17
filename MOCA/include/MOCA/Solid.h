#ifndef MOCA_SOLID_H
#define MOCA_SOLID_H

#include "AbstractBody.h"
#include <armadillo>

/**
 * @class Solid
 * @brief Un solide est un corps qui possede un volume.
 *
 * Un solide est un corps possedant un volume, et donc un potentiel de rotation.
 * C'est donc plus complique a implementer qu'une simple particule.
 */
class Solid : public AbstractBody {

public:
    Solid(bool isHollow = false);
    virtual ~Solid();

    // Getters et setters
    // Retourne le moment d'inertie autour des 3 axes principaux, x, y et z
    virtual arma::mat33 getInertia() const = 0;
    virtual double getVolume() const = 0;

    const arma::vec3 &angularPosition() { return _angularPosition; }
    const arma::vec3 &angularVelocity() { return _angularVelocity; }
    bool isHollow() const { return _isHollow; }

    void setAngularPosition(const arma::vec3 &apos) { _angularPosition = apos; }
    void setAngularVelocity(const arma::vec3 &avel) { _angularVelocity = avel; }

    // Helpers
    void setAngularPosition(double tx, double ty, double tz);
    void setAngularVelocity(double wx, double wy, double wz);

protected:
    arma::vec3 _angularPosition;
    arma::vec3 _angularVelocity;
    // Indique si la boule est creuse
    bool _isHollow;
};

#endif // MOCA_SOLID_H
