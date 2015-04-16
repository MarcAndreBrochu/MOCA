#ifndef MOCA_ABSTRACT_BODY_H
#define MOCA_ABSTRACT_BODY_H

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

    // Getters et setters
    double getMass() const { return _mass; }
    const arma::vec &getPosition() const { return _position; }
    const arma::vec &getVelocity() const { return _velocity; }

    void setMass(double mass) { _mass = mass; }
    void setPosition(const arma::vec &position) { _position = position; }
    void setVelocity(const arma::vec &velocity) { _velocity = velocity; }

protected:
    double _mass;
    arma::vec _position;
    arma::vec _velocity;

    /** Les forces qui s'appliquent sur le corps */
    std::vector<arma::vec> _forces;
    /** Les impulsions qui s'appliquent sur le corps */
    std::vector<arma::vec> _impulsions;
};

#endif // MOCA_ABSTRACT_BODY_H
