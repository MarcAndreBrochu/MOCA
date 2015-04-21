#ifndef MOCA_INTEGRATOR_H
#define MOCA_INTEGRATOR_H

#include <armadillo>

class AbstractBody;

/**
 * @class Integrator
 * @brief Methodes statiques d'integration
 */
class Integrator {

public:
    Integrator();
    ~Integrator();

    struct Status {
        arma::vec3 p; // position
        arma::vec3 v; // velocity
        arma::vec3 a; // accel
    };

    //Implementation de la méthode de Simpson pour intégrer numériquement
    static float simpson(int, int, int);

    /**
      *@brief Implementation de l'algorithme "Velocity Verlet"
      *@param On a besoin de :
      *  position initiale x(t)
      *  vitesse initiale v(t)
      *  accélération a(t) et a(t+step)
      * @author LBL
      */
    static Status verlet(const arma::vec3 &x, const arma::vec3 &v, const arma::vec3 &a, const arma::vec3 &a2, double step);
    static Status verlet(const AbstractBody &body, double step);

private:
    static float yFonct(double);
};

#endif // MOCA_INTEGRATOR_H
