#ifndef MOCA_BOX_H
#define MOCA_BOX_H

#include "Solid.h"

#include <iostream>

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

    void setDimensionX(double dx) { _dimensionX = dx; _listVertices = getLocalVertices();}
    void setDimensionY(double dy) { _dimensionX = dy; _listVertices = getLocalVertices();}
    void setDimensionZ(double dz) { _dimensionX = dz; _listVertices = getLocalVertices();}

    //Les trois axes de la boéte normalisés et positionnés au centre de la boéte
    arma::vec3 getWorldXAxis() const;
    arma::vec3 getWorldYAxis() const;
    arma::vec3 getWorldZAxis() const;
    arma::mat getLocalVertices();

private:
    /** Represente la longueur dans un systeme cartesien de la main droite */
    double _dimensionX;
    /** Represente la hauteur dans un systeme cartesien de la main droite */
    double _dimensionY;
    /** Represente la largeur dans un systeme cartesien de la main droite */
    double _dimensionZ;

    //Liste des vertices de la boite
    arma::mat _listVertices;
};

#endif // MOCA_BOX_H
