#ifndef MOCA_INTEGRATOR_H
#define MOCA_INTEGRATOR_H

#include <armadillo>

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
      static arma::vec3 verlet(arma::vec3 &, arma::vec3 &, arma::vec3, int);

  private:
      static float yFonct(float);


};

#endif // MOCA_INTEGRATOR_H
