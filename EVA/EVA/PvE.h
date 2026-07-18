#pragma once
#include <vector>
#include "Entity.h"
#include "Survival.h"
#include "Battle.h"
#include "Shoot.h"

class PvE {
public:
    // Collisions tirs joueur > ennemis
    static void handleCollisionsJoueurShoot(
        std::vector<Shoot*>& shoot,
        std::vector<Ennemi*>& ennemis,
        int& score);

    static void handleCollisionsJoueurShootS(
        std::vector<Shoot*>& shoot,
        std::vector<Soldat*>& soldat,
        int& score);

    // Collisions tirs ennemis > joueur
    static void handleCollisionsEnnemiShoot(
        std::vector<Shoot*>& shoot,
        Entity* player,
        int& vies);

    // Collision directe joueur < > ennemi (contact)
    static void handleCollisionsJoueurEnnemis(
        Entity* player,
        std::vector<Ennemi*>& ennemis,
        int& vies);

    // Retourne true si le joueur a été touché ce tick (utile pour déclencher des effets indépendants des vies)
    static bool handleCollisionsJoueurSoldat(
        Entity* player,
        std::vector<Soldat*>& soldat,
        int& vies);

    // Collision tirs de soldat (SoldatProjectile) > joueur ; seule l'équipe Orange peut toucher le joueur
    static bool handleCollisionsJoueurProjectilesSoldat(
        Entity* player,
        std::vector<SoldatProjectile*>& projectiles);

    // Tir automatique des ennemis
    static void enemyShoot(
        std::vector<Shoot*>& shoot,
        std::vector<Ennemi*>& ennemis,
        float dt, float now);

    static void soldatShoot(
        std::vector<Shoot*>& shoot,
        std::vector<Soldat*>& soldat,
        float dt, float now);

    static void ResolveEnnemiCollisions(std::vector<Ennemi*>& ennemis);
    static void ResolveSoldatCollisions(std::vector<Soldat*>& soldat);

    // Nettoyage des ennemis morts
    static void cleanupEnnemis(std::vector<Ennemi*>& ennemis);
    static void cleanupSoldat(std::vector<Soldat*>& soldat);
};