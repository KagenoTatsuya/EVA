// PvE.cpp
#include "PvE.h"
#include <cstdlib>
#include <algorithm>


void PvE::handleCollisionsJoueurShoot(std::vector<Shoot*>& shoot, std::vector<Ennemi*>& ennemis, int& score) {
    for (Shoot* s : shoot) {
        if (!s || !s->alive || s->type != ShootType::Player) continue;

        for (Ennemi* e : ennemis) {
            if (!e || !e->alive) continue;

            if (s->rect.getGlobalBounds().findIntersection(e->rect.getGlobalBounds())) {
                e->alive = false;
                s->alive = false;
                score += 10; // points par ennemi tué, ajuste si besoin
                break;
            }
        }
    }
}

void PvE::handleCollisionsEnnemiShoot(
    std::vector<Shoot*>& shoot,
    Entity* player,
    int& vies)
{
    sf::FloatRect playerBounds = player->rect.getGlobalBounds();
    for (Shoot* s : shoot) {
        if (!s->alive || !s->isEnnemi) continue; // on ne garde que les tirs ennemis vivants
        sf::FloatRect tirBounds = s->rect.getGlobalBounds();
        if (tirBounds.findIntersection(playerBounds)) {
            s->alive = false;
            vies--;
            if (vies <= 0)
                player->alive = false;
            break;
        }
    }
}

void PvE::handleCollisionsJoueurEnnemis(Entity* player, std::vector<Ennemi*>& ennemis, int& vies) {
    sf::FloatRect playerBounds = player->rect.getGlobalBounds();

    for (Ennemi* e : ennemis) {
        if (!e || !e->alive) continue;

        if (e->rect.getGlobalBounds().findIntersection(playerBounds)) {
            vies -= 1;
            e->alive = false; // l'ennemi kamikaze meurt en touchant le joueur
            break; // un seul coup par frame max, l'invincibilité gère le reste
        }
    }
}

void PvE::enemyShoot(
    std::vector<Shoot*>& shoot,
    std::vector<Ennemi*>& ennemis,
    float dt, float now)
{
    static float shootTimer = 0.f;
    shootTimer += dt;

    if (shootTimer < 2.f) return;
    shootTimer = 0.f;

    for (Ennemi* e : ennemis) {
        if (!e->alive) continue;
        if (rand() % 100 >= 20) continue;

        sf::Vector2f pos = e->rect.getPosition();
        sf::Vector2f size = e->rect.getSize();

        Shoot* s = new Shoot(
            pos.x + size.x,
            pos.y + size.y / 2.f,
            -300.f, 0.f,
            ShootType::Player // seul membre dispo, isEnnemi fera la distinction
        );
        s->isEnnemi = true;
        shoot.push_back(s);
    }
}

void PvE::ResolveEnnemiCollisions(std::vector<Ennemi*>& ennemis)
{
    for (size_t i = 0; i < ennemis.size(); ++i) {
        Ennemi* a = ennemis[i];
        if (!a->alive) continue;

        for (size_t j = i + 1; j < ennemis.size(); ++j) {
            Ennemi* b = ennemis[j];
            if (!b->alive) continue;

            sf::FloatRect boundsA = a->rect.getGlobalBounds();
            sf::FloatRect boundsB = b->rect.getGlobalBounds();

            auto intersection = boundsA.findIntersection(boundsB);
            if (!intersection.has_value()) continue;

            sf::FloatRect overlap = intersection.value();

            // Centres pour connaitre la direction de poussée
            sf::Vector2f centerA = boundsA.position + boundsA.size / 2.f;
            sf::Vector2f centerB = boundsB.position + boundsB.size / 2.f;

            // On pousse sur l'axe où le chevauchement est le plus petit
            // (évite les à-coups quand les ennemis se croisent en diagonale)
            if (overlap.size.x < overlap.size.y) {
                float push = overlap.size.x / 2.f;
                if (centerA.x < centerB.x) {
                    a->rect.move(sf::Vector2f(-push, 0.f));
                    b->rect.move(sf::Vector2f(push, 0.f));
                }
                else {
                    a->rect.move(sf::Vector2f(push, 0.f));
                    b->rect.move(sf::Vector2f(-push, 0.f));
                }
            }
            else {
                float push = overlap.size.y / 2.f;
                if (centerA.y < centerB.y) {
                    a->rect.move(sf::Vector2f(0.f, -push));
                    b->rect.move(sf::Vector2f(0.f, push));
                }
                else {
                    a->rect.move(sf::Vector2f(0.f, push));
                    b->rect.move(sf::Vector2f(0.f, -push));
                }
            }
        }
    }
}

void PvE::cleanupEnnemis(std::vector<Ennemi*>& ennemis) {
    for (auto it = ennemis.begin(); it != ennemis.end();) {
        if (!(*it)->alive) {
            delete* it;
            it = ennemis.erase(it);
        }
        else {
            ++it;
        }
    }
}

//=============================================================SOLDAT===============================================

void PvE::handleCollisionsJoueurShootS(std::vector<Shoot*>& shoot, std::vector<Soldat*>& soldat, int& score) {
    for (Shoot* s : shoot) {
        if (!s || !s->alive || s->type != ShootType::Player) continue;

        for (Soldat* t : soldat) {
            if (!t || !t->alive) continue;

            if (s->rect.getGlobalBounds().findIntersection(t->rect.getGlobalBounds())) {
                t->alive = false;
                s->alive = false;
                score += 10; // points par ennemi tué, ajuste si besoin
                break;
            }
        }
    }
}

void PvE::handleCollisionsJoueurSoldat(Entity* player, std::vector<Soldat*>& soldat, int& vies) {
    sf::FloatRect playerBounds = player->rect.getGlobalBounds();

    for (Soldat* t : soldat) {
        if (!t || !t->alive) continue;
        if (t->GetTeam() == Team::Bleu) continue; // allié : aucun dégât au contact

        if (t->rect.getGlobalBounds().findIntersection(playerBounds)) {
            vies -= 1;
            t->alive = false;
            break;
        }
    }
}
void PvE::soldatShoot(
    std::vector<Shoot*>& shoot,
    std::vector<Soldat*>& soldat,
    float dt, float now)
{
    static float shootTimer = 0.f;
    shootTimer += dt;

    if (shootTimer < 2.f) return;
    shootTimer = 0.f;

    for (Soldat* t : soldat) {
        if (!t->alive) continue;
        if (rand() % 100 >= 20) continue;

        sf::Vector2f pos = t->rect.getPosition();
        sf::Vector2f size = t->rect.getSize();

        Shoot* s = new Shoot(
            pos.x + size.x,
            pos.y + size.y / 2.f,
            -300.f, 0.f,
            ShootType::Player // seul membre dispo, isEnnemi fera la distinction
        );
        s->isSoldat = true;
        shoot.push_back(s);
    }
}

void PvE::ResolveSoldatCollisions(std::vector<Soldat*>& soldat)
{
    for (size_t i = 0; i < soldat.size(); ++i) {
        Soldat* a = soldat[i];
        if (!a->alive) continue;

        for (size_t j = i + 1; j < soldat.size(); ++j) {
            Soldat* b = soldat[j];
            if (!b->alive) continue;

            sf::FloatRect boundsA = a->rect.getGlobalBounds();
            sf::FloatRect boundsB = b->rect.getGlobalBounds();

            auto intersection = boundsA.findIntersection(boundsB);
            if (!intersection.has_value()) continue;

            sf::FloatRect overlap = intersection.value();

            // Centres pour connaitre la direction de poussée
            sf::Vector2f centerA = boundsA.position + boundsA.size / 2.f;
            sf::Vector2f centerB = boundsB.position + boundsB.size / 2.f;

            // On pousse sur l'axe où le chevauchement est le plus petit
            // (évite les à-coups quand les ennemis se croisent en diagonale)
            if (overlap.size.x < overlap.size.y) {
                float push = overlap.size.x / 2.f;
                if (centerA.x < centerB.x) {
                    a->rect.move(sf::Vector2f(-push, 0.f));
                    b->rect.move(sf::Vector2f(push, 0.f));
                }
                else {
                    a->rect.move(sf::Vector2f(push, 0.f));
                    b->rect.move(sf::Vector2f(-push, 0.f));
                }
            }
            else {
                float push = overlap.size.y / 2.f;
                if (centerA.y < centerB.y) {
                    a->rect.move(sf::Vector2f(0.f, -push));
                    b->rect.move(sf::Vector2f(0.f, push));
                }
                else {
                    a->rect.move(sf::Vector2f(0.f, push));
                    b->rect.move(sf::Vector2f(0.f, -push));
                }
            }
        }
    }
}

void PvE::cleanupSoldat(std::vector<Soldat*>& soldat) {
    for (auto it = soldat.begin(); it != soldat.end();) {
        if (!(*it)->alive) {
            delete* it;
            it = soldat.erase(it);
        }
        else {
            ++it;
        }
    }
}