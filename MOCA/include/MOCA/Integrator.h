#ifndef MOCA_INTEGRATOR_H
#define MOCA_INTEGRATOR_H

namespace Integrator {

/**
 * @class Simpson
 * @brief Integre numeriquement des donnees sur un intervalle.
 * Integre numeriquement en utilisant la methode de Simpson
 */
class Simpson {

public:
    Simpson();
    ~Simpson();

private:
};

/**
 * @class Verlet
 * @brief Integre numeriquement des donnes sur un intervalle
 * Integre numeriquement en utlisant la methode Verlet
 */
class Verlet {

public:
    Verlet();
    ~Verlet();

private:
};

} // namespace Integrator

#endif // MOCA_INTEGRATOR_H
