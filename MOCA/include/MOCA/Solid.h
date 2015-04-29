#ifndef MOCA_SOLID_H
#define MOCA_SOLID_H

#include <armadillo>
#include <unordered_map>
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
    Solid(bool isHollow = false);
    virtual ~Solid();

    /**
     * @brief Applique une impulsion angulaire sur l'objet.
     * Une impulsion est comme une force, mais appliquee sur un intervale de temps.
     * Dans notre cas, cet interval est minuscule : le temps d'une frame. Apres 1
     * frame, l'impulsion n'est plus appliquee.
     * @param aimp L'impulsion angulaire a appliquer
     * @author MAB
     */
    void applyAngularImpulse(const arma::vec3 aimp);

    /**
     * @brief Appliqe une force rotationnelle sur l'objet.
     * Une force est constament appliquee sur l'objet, meme apres plusieurs frames.
     * Cela pourrait etre par exemple une force localisee sur un seul objet, contrairemement
     * a par exemple la gravite qui s'applique sur tous les objets.
     * @param torque La torque a appliquer au corps
     * @return L'index de la torque appliquee sur le corps
     */
    uint applyTorque(const arma::vec3 torque);

    /**
     * @brief Arrete d'appliquer la torque #tid sur l'objet
     * @param tid L'index de la torque
     */
    void removeTorque(uint tid);

    // Getters et setters
    // Retourne le moment d'inertie autour des 3 axes principaux, x, y et z
    virtual arma::mat33 getInertia() const = 0;
    virtual double getVolume() const = 0;

    const arma::vec3 &getAngularPosition() { return _angularPosition; }
    const arma::vec3 &getAngularVelocity() { return _angularVelocity; }
    const arma::vec3 &getAngularImpulse() { return _angularImpulse; }
    bool isHollow() const { return _isHollow; }

    arma::vec3 getAngularAcceleration() const;

    void setAngularPosition(const arma::vec3 &apos) { _angularPosition = apos; }
    void setAngularVelocity(const arma::vec3 &avel) { _angularVelocity = avel; }

    virtual void resetImpulse();

    // Helpers
    void setAngularPosition(double tx, double ty, double tz);
    void setAngularVelocity(double wx, double wy, double wz);

protected:
    arma::vec3 _angularPosition;
    arma::vec3 _angularVelocity;

    /** Les forces qui s'appliquent sur le corps */
    std::unordered_map<uint, arma::vec3> _torques;
    // Somme des torques
    arma::vec3 _sumTorques;
    /** Les impulsions angulaires qui s'appliquent sur le corps */
    arma::vec3 _angularImpulse;

    // Indique si le solide est creux
    bool _isHollow;

private:
    uint _lastKeyAssigned;
};

#endif // MOCA_SOLID_H
