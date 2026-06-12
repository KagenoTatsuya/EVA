#include "Entity.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>


float M_PI = 3.14159265358979323846;   // pi


// ===== Entity =====
Entity::Entity(sf::RectangleShape r, float vx, float vy)
    : rect(r), vx(vx), vy(vy)
{
    width = r.getSize().x;
    height = r.getSize().y;
    posx = r.getPosition().x;
    posy = r.getPosition().y;
    alive = true;
}

void Entity::Update(float dt, sf::Vector2u windowSize, std::vector<Block*>& blocks) {
    // Basic movement: position += velocity * deltaTime
    rect.move(sf::Vector2(vx * dt, vy * dt));
}

// ===== Player =====
Joueur::Joueur(float x, float y)
    : Entity(sf::RectangleShape(sf::Vector2f(32.f, 48.f)), 0.f, 0.f) {

    rect.setPosition(sf::Vector2(x, y));
    rect.setFillColor(sf::Color::Blue);

    if (!animator.LoadTexture("character-spritesheet.png")) {
        std::cout << "Erreur chargement spritesheet\n";
    }

    // Chaque frame fait 32x32
    const int W = 64;
    const int H = 64;

    // Animation BAS
    std::vector<sf::IntRect> upFrames = {
        sf::IntRect({0 * W, 8 * H}, {W, H}),
        sf::IntRect({1 * W, 8 * H}, {W, H}),
        sf::IntRect({2 * W, 8 * H}, {W, H}),
        sf::IntRect({3 * W, 8 * H}, {W, H})
    };

    animator.AddAnimation("up", upFrames, 0.12f);

    // Animation GAUCHE
    std::vector<sf::IntRect> leftFrames = {
        sf::IntRect({0 * W, 9 * H}, {W, H}),
        sf::IntRect({1 * W, 9 * H}, {W, H}),
        sf::IntRect({2 * W, 9 * H}, {W, H}),
        sf::IntRect({3 * W, 9 * H}, {W, H})
    };

    animator.AddAnimation("left", leftFrames, 0.12f);

    // Animation HAUT
    std::vector<sf::IntRect> downFrames = {
        sf::IntRect({0 * W, 10 * H}, {W, H}),
        sf::IntRect({1 * W, 10 * H}, {W, H}),
        sf::IntRect({2 * W, 10 * H}, {W, H}),
        sf::IntRect({3 * W, 10 * H}, {W, H})
    };

    animator.AddAnimation("down", downFrames, 0.12f);

    // Animation DROITE
    std::vector<sf::IntRect> rightFrames = {
        sf::IntRect({0 * W, 11 * H}, {W, H}),
        sf::IntRect({1 * W, 11 * H}, {W, H}),
        sf::IntRect({2 * W, 11 * H}, {W, H}),
        sf::IntRect({3 * W, 11 * H}, {W, H})
    };

    animator.AddAnimation("right", rightFrames, 0.12f);

    // ======================= Animation Idle ==============================

    // BAS
    std::vector<sf::IntRect> upIdle = {
        sf::IntRect({0 * W, 22 * H}, {W, H}),
        sf::IntRect({1 * W, 22 * H}, {W, H})
    };

    animator.AddAnimation("upIdle", upIdle, 0.12f);

    // GAUCHE
    std::vector<sf::IntRect> leftIdle = {
        sf::IntRect({0 * W, 23 * H}, {W, H}),
        sf::IntRect({1 * W, 23 * H}, {W, H})
    };

    animator.AddAnimation("leftIdle", leftIdle, 0.12f);

    // HAUT
    std::vector<sf::IntRect> downIdle = {
        sf::IntRect({0 * W, 24 * H}, {W, H}),
        sf::IntRect({1 * W, 24 * H}, {W, H})
    };

    animator.AddAnimation("downIdle", downIdle, 0.12f);

    // DROITE
    std::vector<sf::IntRect> rightIdle = {
        sf::IntRect({0 * W, 25 * H}, {W, H}),
        sf::IntRect({1 * W, 25 * H}, {W, H})
    };

    animator.AddAnimation("rightIdle", rightIdle, 0.12f);

    animator.Play("downIdle"); // animation par défaut
}

void Joueur::Update(float dt, sf::Vector2u windowSize, std::vector<Block*>& blocks) {
    // Input
    vx = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) * 200.f;

    vy = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) * 200.f;

    

    // Choix de l’animation selon direction
    if (vy > 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        lastDirection = Direction::DOWN;
        animator.Play("down");
    }
    else if (vy < 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
        lastDirection = Direction::UP;
        animator.Play("up");
    }
    else if (vx < 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        lastDirection = Direction::LEFT;
        animator.Play("left");
    }
    else if (vx > 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        lastDirection = Direction::RIGHT;
        animator.Play("right");
    }
    else {
        switch (lastDirection) {
        case Joueur::Direction::DOWN:
            animator.Play("downIdle");
            break;
        case Joueur::Direction::LEFT:
            animator.Play("leftIdle");
            break;
        case Joueur::Direction::RIGHT:
            animator.Play("rightIdle");
            break;
        case Joueur::Direction::UP:
            animator.Play("upIdle");
            break;
        }
    }

    // Déplacement
    rect.move(sf::Vector2(vx * dt, vy * dt));

    // Récupérer position et taille
    posx = rect.getPosition().x + 15.f; // offset hitbox comme dans Render
    posy = rect.getPosition().y + 13.f;

    // Résoudre les collisions
    ResolveCollisions(blocks);

    std::cout << posx << "_" << posy << std::endl;        // Coordonée joueur 

    // Réappliquer la position corrigée sur rect
    rect.setPosition(sf::Vector2f(posx - 15.f, posy - 13.f));

    // Mise à jour animation
    animator.Update(dt);
    if (animator.sprite)
        animator.sprite->setPosition(rect.getPosition());
}

void Joueur::Render(sf::RenderTarget* target) {
    //target->draw(rect);
    if (animator.sprite)
        target->draw(*animator.sprite);

    sf::RectangleShape hitbox(sf::Vector2f(width, height));
    hitbox.setPosition(sf::Vector2f(rect.getPosition().x + 15, rect.getPosition().y + 13));
    hitbox.setFillColor(sf::Color::Transparent);
    //hitbox.setOutlineColor(sf::Color::Green);
    //hitbox.setOutlineThickness(2.f);
    target->draw(hitbox);
}

void Joueur::ResolveCollisions(std::vector<Block*>& blocks) {
    for (auto* bl : blocks) {
        if (bl->GetBlockType() != "MBlock") continue;

        float bL = bl->GetPosX();
        float bR = bl->GetRightX();     
        float bT = bl->GetPosY();
        float bB = bl->GetBottomY();    

        float pL = posx;
        float pR = posx + width;    // width = 32
        float pT = posy;
        float pB = posy + height;   // height = 48

        // Vérifier le chevauchement AABB
        bool overlapX = pR > bL && pL < bR;
        bool overlapY = pB > bT && pT < bB;

        if (!overlapX || !overlapY) continue;

        // Calculer la pénétration sur chaque axe
        float penLeft = pR - bL;  // overlap si on vient de gauche
        float penRight = bR - pL;  // overlap si on vient de droite
        float penTop = pB - bT;  // overlap si on vient du haut
        float penBot = bB - pT;  // overlap si on vient du bas

        // Résoudre sur l'axe de moindre pénétration
        float minPenX = std::min(penLeft, penRight);
        float minPenY = std::min(penTop, penBot);

        if (minPenX < minPenY) {
            // Correction horizontale
            if (penLeft < penRight)
                posx = bL - width;   // vient de gauche -> pousse à gauche
            else
                posx = bR;           // vient de droite -> pousse à droite
            vx = 0;
        }
        else {
            // Correction verticale
            if (penTop < penBot)
                posy = bT - height;  // vient du haut -> pose sur le bloc
            else
                posy = bB;           // vient du bas -> colle au plafond
            vy = 0;
        }
    }
}