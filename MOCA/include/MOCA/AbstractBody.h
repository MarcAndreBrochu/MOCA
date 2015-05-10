#ifndef MOCA_ABSTRACT_BODY_H
#define MOCA_ABSTRACT_BODY_H

#include <unordered_map>
#include <vector>
#include <armadillo>
#include <numeric>

/**
 * @class AbstractBody
 * @brief Represente un corps rigide quelconque.
 *
 * Represente un corps rigide quelconque, que ce soit une particule ou encore
 * un volume.
 */

 typedef unsigned int uint;

class AbstractBody {

public:
    AbstractBody();
    virtual ~AbstractBody();

    /**
     * @brief Applique une impulsion sur l'objet.
     * Une impulsion est comme une force, mais appliquee sur un intervale de temps.
     * Dans notre cas, cet interval est minuscule : le temps d'une frame. Apres 1
     * frame, l'impulsion n'est plus appliquee.
     *
     * @param imp L'impulsion a appliquer
     * @author MAB
     */
    void applyImpulse(const arma::vec3 &imp);

    /**
     * @brief Appliqe une force sur l'objet.
     * Une force est constament appliquee sur l'objet, meme apres plusieurs frames.
     * Cela pourrait etre par exemple une force localisee sur un seul objet, contrairemement
     * a par exemple la gravite qui s'applique sur tous les objets.
     *
     * @param force La force a appliquer
     * @return L'index de la force en memoire
     * @author MAB
     */
    uint applyForce(const arma::vec3 &force);

    /**
     * @brief Arrete d'appliquer la force #fid sur l'objet.
     * @param fid L'index de la force en memoire
     * @author MAB
     */
    void removeForce(uint fid);

    // Getters et setters
    double getMass() const { return _mass; }
    const arma::vec3 &getSumForces() const { return _sumForces; }
    const arma::vec3 &getImpulse() const { return _impulse; }
    const arma::vec3 &getPosition() const { return _position; }
    const arma::vec3 &getVelocity() const { return _velocity; }
    bool isFixed() const { return _isFixed; }

    void setMass(double mass) {
        _mass = mass;
        _oldMass = mass;
    }
    void setPosition(const arma::vec3 &position) { _position = position; }
    void setVelocity(const arma::vec3 &velocity) { _velocity = velocity; }
    void setIsFixed(bool fixed) {

        _isFixed = fixed;
        if (fixed) _mass = 1e+15; // vraiment grosse masse
        else _mass = _oldMass;
    }

    arma::vec3 getAcceleration() const;
    virtual void resetImpulse() { _impulse.zeros(); }

    // Helpers
    void setPosition(double x, double y, double z);
    void setVelocity(double vx, double vy, double vz);

protected:
    double _mass;
    double _oldMass;
    arma::vec3 _position;
    arma::vec3 _velocity;

    /** Les forces qui s'appliquent sur le corps */
    std::unordered_map<uint, arma::vec3> _forces;
    //Somme des forces
    arma::vec3 _sumForces;
    /** Les impulsions qui s'appliquent sur le corps */
    arma::vec3 _impulse;

    // Indique si le corps n'est pas affecte par les forces et les impulsions
    bool _isFixed;

private:
    // Derniere key associee aux forces
    uint _lastKeyAssigned;
};

#endif // MOCA_ABSTRACT_BO
