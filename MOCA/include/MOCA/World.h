#ifndef MOCA_WORLD_H
#define MOCA_WORLD_H

#include <unordered_map>
#include <armadillo>

class AbstractBody;

/**
 * @class World
 * @brief World est un espace dans lequel les objets simules evoluent.
 *
 * Comme son nom l'indique, World est un espace abstrait qui permet de quelque peu
 * 'centraliser' la simulation et donne un endroit ou il est possible de mettre en
 * relation tous les objets qui sont presentement existants. C'est la classe centrale
 * du moteur de physique au complet.
 */
class World {

public:
    World();
    ~World();

    /**
     * @brief Ajoute un corps rigide au World.
     * @param body Le corps a ajouter
     * @return L'index du corps ajoute
     */
    uint addBody(AbstractBody *body);

    // fonction pour mettre à jour le monde et ses objets
    void updateWorld();

private:
    // Key du dernier corps ajoute au monde
    uint _lastKeyAssigned;

    /** Liste qui contient tous les corps a simuler */
    std::unordered_map<uint, AbstractBody *> _bodies;
    /** Contient les forces qui s'appliquent en general sur les corps */
    std::unordered_map<uint, arma::vec> _forces;

    //L'intervalle de temps d'une frame en millisecondes
    double _timeStep;

};

#endif // MOCA_WORLD_H
