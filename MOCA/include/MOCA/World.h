#ifndef MOCA_WORLD_H
#define MOCA_WORLD_H

#include <vector>
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

private:
    /** Liste qui contient tous les corps a simuler */
    std::vector<AbstractBody *> _bodies;
    /** Contient les forces qui s'appliquent en general sur les corps */
    std::vector<arma::vec> _forces;
};

#endif // MOCA_WORLD_H
