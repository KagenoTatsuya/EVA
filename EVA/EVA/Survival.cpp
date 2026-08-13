#include "Survival.h"
#include "Deplacement.h"
#include <cmath>
#include <numbers>

namespace {
    const std::array<std::string, 2> kEnnemiSpritesheets = {
        "assets/pictures/character-spritesheetZ.png",
        "assets/pictures/character-spritesheetS.png"
    };

    std::string GetRandomSpritesheet() {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<size_t> dist(0, kEnnemiSpritesheets.size() - 1);
        return kEnnemiSpritesheets[dist(rng)];
    }
}

Ennemi::Ennemi(float x, float y, Pattern p)
    : Survival(sf::RectangleShape(sf::Vector2f(32.f, 48.f)), 0.f, -1.f)
    , pattern(p)
    , timeAlive(0.f)
    , phaseX(0.f)
    , phaseY(0.f)
    , cycleX(0)
    , center{ 720.f, y }
{
    rect.setPosition({ x, y });

    hasSprite = animator.LoadTexture(GetRandomSpritesheet());

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

    animator.Play("down"); // animation par défaut
}

void Survival::Update(float dt, sf::Vector2f cible) {
    // comportement par défaut, ou vide
}

void Ennemi::Update(float dt, sf::Vector2f cible) {
    timeAlive += dt;

    const float minX = -150.f;
    const float maxX = 1920.f;
    const float minY = -150.f;
    const float maxY = 1920.f;
    const float centerX = (minX + maxX) / 2.f;
    const float amplitudeX = (maxX - minX) / 2.f;

    sf::Vector2f pos = rect.getPosition();
    sf::Vector2f size = rect.getSize();
    sf::Vector2f oldPos = pos;

    switch (pattern) {
    case Pattern::Kamikaze: {
        pos = Deplacement::getPointArrive(pos, cible, 100.f * dt, minX, minY, maxX, maxY);
        break;
    }
    }

    if (timeAlive >= 3.f) {
        pos.x = std::max(minX, std::min(pos.x, maxX - size.x));
        pos.y = std::max(minY, std::min(pos.y, maxY - size.y));
    }

    // Direction + changement d'animation
    sf::Vector2f delta = pos - oldPos;
    Direction newDirection = lastDirection;

    if (std::abs(delta.x) > std::abs(delta.y)) {
        newDirection = (delta.x > 0.f) ? Direction::RIGHT : Direction::LEFT;
    }
    else if (std::abs(delta.y) > 0.01f) {
        newDirection = (delta.y > 0.f) ? Direction::DOWN : Direction::UP;
    }

    if (newDirection != lastDirection) {
        lastDirection = newDirection;
        switch (lastDirection) {
        case Direction::UP:    animator.Play("up");    break;
        case Direction::DOWN:  animator.Play("down");  break;
        case Direction::LEFT:  animator.Play("left");  break;
        case Direction::RIGHT: animator.Play("right"); break;
        }
    }

    rect.setPosition(pos);
    animator.Update(dt);
    if (animator.sprite)
        animator.sprite->setPosition(rect.getPosition());
}

void Ennemi::Render(sf::RenderTarget* target) {
    if (animator.sprite) {
        animator.sprite->setPosition(rect.getPosition());
        target->draw(*animator.sprite);
    }
    else {
        rect.setFillColor(sf::Color::Red);
        target->draw(rect);
    }
}

Ennemi::~Ennemi() {
}

void Ennemi::ResolveCollisions(std::vector<Block*>& blocks) {
    sf::FloatRect bounds = rect.getGlobalBounds();
    for (Block* b : blocks) {
        if (b->GetBlockType() != "MBlock") continue; // ne collisionne qu'avec les vrais murs

        sf::FloatRect bb = b->rect.getGlobalBounds();
        if (auto overlap = bounds.findIntersection(bb)) {
            float overlapX = std::min(bounds.position.x + bounds.size.x, bb.position.x + bb.size.x)
                - std::max(bounds.position.x, bb.position.x);
            float overlapY = std::min(bounds.position.y + bounds.size.y, bb.position.y + bb.size.y)
                - std::max(bounds.position.y, bb.position.y);
            sf::Vector2f pos = rect.getPosition();
            if (overlapX < overlapY) {
                pos.x += (bounds.position.x < bb.position.x) ? -overlapX : overlapX;
            }
            else {
                pos.y += (bounds.position.y < bb.position.y) ? -overlapY : overlapY;
            }
            rect.setPosition(pos);
            bounds = rect.getGlobalBounds();
        }
    }
}

//===============================================SPAWN ENNEMI================================================

float EnnemiSpawner::GetWaveInterval() const {
    if (m_totalSpawned < 50) return 10.f;
    if (m_totalSpawned < 70) return 5.f;
    return 1.f;
}

void EnnemiSpawner::Update(float dt, std::vector<Ennemi*>& ennemis, float spawnX, float spawnY) {
    if (m_totalSpawned >= m_maxTotal) return;

    // Si une vague est en cours, on spawn un ennemi toutes les 0.5 sec
    if (m_enemiesQueued > 0) {
        m_spawnTimer -= dt;
        if (m_spawnTimer <= 0.f) {
            ennemis.push_back(new Ennemi(spawnX, spawnY, Pattern::Kamikaze));
            m_totalSpawned++;
            m_enemiesQueued--;
            m_spawnTimer = 1.f;

            if (m_totalSpawned >= m_maxTotal) return;
        }
        return; // on attend que la vague finisse avant de relancer le timer de vague
    }

    // Pas de vague en cours : on attend l'intervalle entre vagues
    m_waveTimer -= dt;
    if (m_waveTimer <= 0.f) {
        m_enemiesQueued = 10;
        m_spawnTimer = 0.f; // démarre le premier spawn immédiatement
        m_waveTimer = GetWaveInterval();
    }
}