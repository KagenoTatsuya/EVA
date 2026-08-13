#include "Entity.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cmath>
#include "Shoot.h"

#define _USE_MATHS_DEFINE

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

void Entity::Update(float dt, sf::Vector2u windowSize, std::vector<Block*>& blocks, std::vector<Shoot*>& shoot, bool canShoot) {
    // Basic movement: position += velocity * deltaTime
    rect.move(sf::Vector2(vx * dt, vy * dt));
}

// ===== Player =====
Joueur::Joueur(float x, float y)
    : Entity(sf::RectangleShape(sf::Vector2f(32.f, 48.f)), 0.f, 0.f) {

    rect.setPosition(sf::Vector2(x, y));
    rect.setFillColor(sf::Color::Blue);

    if (!animator.LoadTexture("assets/pictures/character-spritesheet.png")) {
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

void Joueur::Update(float dt, sf::Vector2u windowSize, std::vector<Block*>& blocks, std::vector<Shoot*>& shoot, bool canShoot) {
    // Input
    vx = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) * 200.f;

    vy = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) * 200.f;


    // Choix de l'animation selon direction
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

    // Plafonne le dt utilisé pour le mouvement afin d'éviter le tunneling
    // en cas de pic de lag (un dt trop grand ferait sauter le joueur par-dessus un mur)
    float safeDt = std::min(dt, 1.f / 30.f);

    // Récupérer position et taille (offset hitbox comme dans Render)
    posx = rect.getPosition().x + 15.f;
    posy = rect.getPosition().y + 13.f;

    // Déplacement + collision séparés PAR AXE (X puis Y)
    // -> empêche le joueur de glisser à travers un coin ou un mur fin
    posx += vx * safeDt;
    ResolveCollisionsAxis(blocks, true);   // résolution axe X

    posy += vy * safeDt;
    ResolveCollisionsAxis(blocks, false);  // résolution axe Y

    if (canShoot) {
        shootCooldown = std::max(0.f, shootCooldown - dt);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && shootCooldown <= 0.f) {
            sf::Vector2f pos = rect.getPosition();
            sf::Vector2f size = rect.getSize();
            float bvx = 0.f, bvy = 0.f;
            float bx = pos.x, by = pos.y + size.y / 2.f;

            switch (lastDirection) {
            case Direction::RIGHT: bvx = 300.f; bx = pos.x + size.x; break;
            case Direction::LEFT:  bvx = -300.f; bx = pos.x;          break;
            case Direction::DOWN:  bvy = 300.f; bx = pos.x + size.x / 2.f; by = pos.y + size.y; break;
            case Direction::UP:    bvy = -300.f; bx = pos.x + size.x / 2.f; by = pos.y;          break;
            }

            shoot.push_back(new Shoot(bx, by, bvx, bvy, ShootType::Player));
            shootCooldown = 0.3f;
        }
    }

    // Réappliquer la position corrigée sur rect
    rect.setPosition(sf::Vector2f(posx - 15.f, posy - 13.f));

    //std::cout << posx << "_" << posy << std::endl;        // Coordonée joueur 

    // Mise à jour animation
    animator.Update(dt);
    if (animator.sprite)
        animator.sprite->setPosition(rect.getPosition());
}

void Joueur::ResolveCollisionsAxis(std::vector<Block*>& blocks, bool horizontal) {
    for (auto* bl : blocks) {
        if (bl->GetBlockType() != "MBlock") continue;

        float bL = bl->GetPosX();
        float bR = bl->GetRightX();
        float bT = bl->GetPosY();
        float bB = bl->GetBottomY();

        float pL = posx;
        float pR = posx + width;
        float pT = posy;
        float pB = posy + height;

        bool overlapX = pR > bL && pL < bR;
        bool overlapY = pB > bT && pT < bB;

        if (!overlapX || !overlapY) continue;

        if (horizontal) {
            float penLeft = pR - bL;   // vient de gauche
            float penRight = bR - pL;  // vient de droite
            if (penLeft < penRight)
                posx = bL - width;
            else
                posx = bR;
            vx = 0.f;
        }
        else {
            float penTop = pB - bT;    // vient du haut
            float penBot = bB - pT;    // vient du bas
            if (penTop < penBot)
                posy = bT - height;
            else
                posy = bB;
            vy = 0.f;
        }
    }
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

void Joueur::ChangeSpriteSheet(const std::string& spriteSheet) {
    if (!animator.LoadTexture(spriteSheet)) {
        std::cout << "Erreur chargement spritesheet\n";
        return;
    }
    std::string current = animator.GetCurrent(); // ex: "downIdle"
    animator.Play(current.empty() ? "downIdle" : current, true); // force = true -> resynchro le rect sur la nouvelle texture
}

//=================================================================================================================================================

// ===== PNJ =====
PNJ::PNJ(float x, float y, std::string spriteSheet)
    : Entity(sf::RectangleShape(sf::Vector2f(32.f, 48.f)), 0.f, 0.f) {

    rect.setPosition(sf::Vector2(x, y));
    rect.setFillColor(sf::Color::Blue);

    if (!animator.LoadTexture(spriteSheet)) {
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


void PNJ::ResolveCollisions(std::vector<Block*>& blocks) {
    for (auto* bl : blocks) {
        if (bl->GetBlockType() != "MBlock") continue;

        float bL = bl->GetPosX();
        float bR = bl->GetRightX();
        float bT = bl->GetPosY();
        float bB = bl->GetBottomY();

        float pL = posx;
        float pR = posx + width;
        float pT = posy;
        float pB = posy + height;

        bool overlapX = pR > bL && pL < bR;
        bool overlapY = pB > bT && pT < bB;

        if (!overlapX || !overlapY) continue;

        float penLeft = pR - bL;
        float penRight = bR - pL;
        float penTop = pB - bT;
        float penBot = bB - pT;

        float minPenX = std::min(penLeft, penRight);
        float minPenY = std::min(penTop, penBot);

        if (minPenX < minPenY) {
            if (penLeft < penRight) posx = bL - width;
            else                    posx = bR;
            vx = 0;
        }
        else {
            if (penTop < penBot) posy = bT - height;
            else                 posy = bB;
            vy = 0;
        }
    }
}

void PNJ::Render(sf::RenderTarget* target) {
    if (animator.sprite)
        target->draw(*animator.sprite);
}

void PNJ::SetWaypoints(std::vector<sf::Vector2f> waypoints) {
    m_waypoints = waypoints;
    if (!m_waypoints.empty())
        rect.setPosition(m_waypoints[0]);
}

void PNJ::Update(float dt, sf::Vector2u windowSize, std::vector<Block*>& blocks, std::vector<Shoot*>& shoot, bool canShoot) {
    if (m_waypoints.size() < 2) return;

    sf::Vector2f target = m_waypoints[m_currentWaypoint];
    sf::Vector2f pos = rect.getPosition();
    sf::Vector2f dir = target - pos;
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    // Pause active -> on attend
    if (m_isPaused) {
        m_pauseTimer -= dt;

        // Animation idle selon dernière direction
        if (std::abs(dir.x) > std::abs(dir.y)) {
            if (dir.x < 0) animator.Play("leftIdle");
            else           animator.Play("rightIdle");
        }
        else {
            if (dir.y < 0) animator.Play("upIdle");
            else           animator.Play("downIdle");
        }

        if (m_pauseTimer <= 0.f)
            m_isPaused = false;

        animator.Update(dt);
        if (animator.sprite)
            animator.sprite->setPosition(rect.getPosition());
        return;
    }

    if (dist < 2.f) {
        // Déclencher la pause
        m_isPaused = true;
        m_pauseTimer = 2.f;

        // Avancer vers le prochain waypoint
        if (m_movingForward) {
            if (m_currentWaypoint + 1 >= (int)m_waypoints.size()) {
                m_movingForward = false;
                m_currentWaypoint--;
            }
            else {
                m_currentWaypoint++;
            }
        }
        else {
            if (m_currentWaypoint - 1 < 0) {
                m_movingForward = true;
                m_currentWaypoint++;
            }
            else {
                m_currentWaypoint--;
            }
        }
    }
    else {
        // Normaliser et déplacer
        sf::Vector2f move = (dir / dist) * m_speed * dt;
        vx = move.x / dt;
        vy = move.y / dt;
        rect.move(move);

        // Animation selon direction
        if (std::abs(dir.x) > std::abs(dir.y)) {
            if (dir.x < 0) animator.Play("left");
            else           animator.Play("right");
        }
        else {
            if (dir.y < 0) animator.Play("up");
            else           animator.Play("down");
        }
    }

    posx = rect.getPosition().x + 15.f;
    posy = rect.getPosition().y + 13.f;
    ResolveCollisions(blocks);
    rect.setPosition(sf::Vector2f(posx - 15.f, posy - 13.f));

    animator.Update(dt);
    if (animator.sprite)
        animator.sprite->setPosition(rect.getPosition());
}