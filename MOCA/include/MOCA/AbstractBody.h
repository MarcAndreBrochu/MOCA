#ifndef MOCA_ABSTRACT_BODY_H
#define MOCA_ABSTRACT_BODY_H

#include <unordered_map>
#include <vector>
#include <armadillo>

/**
 * @class AbstractBody
 * @brief Represente un corps rigide quelconque.
 *
 * Represente un corps rigide quelconque, que ce soit une particule ou encore
 * un volume.
 */
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
     */
    uint applyForce(const arma::vec3 &force);

    /**
     * @brief Arrete d'appliquer la force #fid sur l'objet.
     * @param fid L'index de la force en memoire
     */
    void removeForce(uint fid);

    // Getters et setters
    double getMass() const { return _mass; }
    const arma::vec3 &getPosition() const { return _position; }
    const arma::vec3 &getVelocity() const { return _velocity; }

    void setMass(double mass) { _mass = mass; }
    void setPosition(const arma::vec3 &position) { _position = position; }
    void setVelocity(const arma::vec3 &velocity) { _velocity = velocity; }

    // Helpers
    void setPosition(double x, double y, double z);
    void setVelocity(double vx, double vy, double vz);

protected:
    double _mass;
    arma::vec3 _position;
    arma::vec3 _velocity;

    /** Les forces qui s'appliquent sur le corps */
    std::unordered_map<uint, arma::vec3> _forces;
    /** Les impulsions qui s'appliquent sur le corps */
    std::vector<arma::vec3> _impulsions;

private:
    // Derniere key associee aux forces
    uint _lastKeyAssigned;
};

#endif // MOCA_ABSTRACT_BODY_H
