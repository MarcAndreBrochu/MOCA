#ifndef MOCA_BOX_H
#define MOCA_BOX_H

#include "Solid.h"

/**
 * @class Box
 * @brief Represente une boite.
 *
 * Represente un solide en forme de boite, c'est-a-dire que ce solide possede une
 * longueur, une largeur et une hauteur qui sont distinctes et invariables.
 */
class Box : public Solid {

public:
    Box();
    virtual ~Box();

    // Getters et setters
    virtual double getInertia() const;
    virtual double getVolume() const;

    double getDimensionX() const { return _dimensionX; }
    double getDimensionY() const { return _dimensionY; }
    double getDimensionZ() const { return _dimensionZ; }

private:
    /** Represente la longueur dans un systeme cartesien de la main droite */
    double _dimensionX;
    /** Represente la hauteur dans un systeme cartesien de la main droite */
    double _dimensionY;
    /** Represente la largeur dans un systeme cartesien de la main droite */
    double _dimensionZ;
};

#endif // MOCA_BOX_H
