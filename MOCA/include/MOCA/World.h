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

    /**
     * @brief Enleve un corps du monde.
     * La mort viens le liberer de ses taches mortelles.
     * @param bid L'index du corps a enlever
     * @param body Le corps a enlever
     */
    void removeBody(uint bid);
    void removeBody(AbstractBody *body);

    /**
     * @brief Applique une acceleration sur tous les objets du World
     * @param force L'accel a appliquer
     * @return L'index de l'accel en memoire
     */
    uint applyAcceleration(const arma::vec3 &accel);

    /**
     * @brief Arreter d'appliquer une accel sur tous les objets
     * @param fid L'index de l'accel en memoire
     */
    void removeAcceleration(uint fid);

    // fonction pour mettre à jour le monde et ses objets
    void updateWorld(double timeStep);

private:
    /**
    * @brief Appelée par updateWorld() Début du processus de l'algorithme général
    * (voir l'organigramme correspondant -- pdf : http://goo.gl/ReUpRH)
    * @param body le corps a simuler
    * @param timeStep la durée d'une frame, soit le temps depuis la dernière update
    * @author LBL
    */
    void updateBody(AbstractBody *body, double timeStep);

    // Key du dernier corps ajoute au monde
    uint _lastKeyAssignedBody;
    uint _lastKeyAssignedForce;

    /** Liste qui contient tous les corps a simuler */
    std::unordered_map<uint, AbstractBody *> _bodies;
    //Somme des accelerations
    arma::vec3 _sumAccel;
    /** Contient les accelerations qui s'appliquent en general sur les corps */
    std::unordered_map<uint, arma::vec3> _accels;
};

#endif // MOCA_WORLD_H
