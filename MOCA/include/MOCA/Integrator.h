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
      //Implementation de la méthode de Simpson pour intégrer numériquement
      static float simpson(int, int, int);


      /**
      *@brief Implementation de l'algorithme "Velocity Verlet"
      *@param On a besoin de :
      *  position initiale x(t)
      *  vitesse initiale v(t)
      *  accélération a(t) et a(t+step)
      */
      static arma::vec3 verlet(arma::vec3 &x, arma::vec3 &v, arma::vec3 a, arma::vec3 a2 double);
      static arma::vec3 verlet(AbstractBody &body, double step);

  private:
      static float yFonct(double);


};

#endif // MOCA_INTEGRATOR_H
