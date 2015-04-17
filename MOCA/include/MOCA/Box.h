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
    /**
     * @brief Construit un objet Box avec certaines dimensions
     * @param dx La dimension en x
     * @param dy La dimension en y
     * @param dz La dimension en z
     */
    Box(double dx, double dy, double dz, bool isHollow = false);
    virtual ~Box();

    // Getters et setters
    virtual arma::mat33 getInertia() const;
    virtual double getVolume() const;

    double getDimensionX() const { return _dimensionX; }
    double getDimensionY() const { return _dimensionY; }
    double getDimensionZ() const { return _dimensionZ; }

    void setDimensionX(double dx) { _dimensionX = dx; }
    void setDimensionY(double dy) { _dimensionX = dy; }
    void setDimensionZ(double dz) { _dimensionX = dz; }

private:
    /** Represente la longueur dans un systeme cartesien de la main droite */
    double _dimensionX;
    /** Represente la hauteur dans un systeme cartesien de la main droite */
    double _dimensionY;
    /** Represente la largeur dans un systeme cartesien de la main droite */
    double _dimensionZ;
};

#endif // MOCA_BOX_H
