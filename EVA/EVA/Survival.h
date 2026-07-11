#pragma once
#include <SFML/Graphics.hpp>
#include "Animator.h"
#include "Block.h"
#include <random>
#include <array>
#include<vector>

class Survival {
public:
    sf::RectangleShape rect;
    float vx, vy;
    bool alive;
    float timeAlive;
    Survival(sf::RectangleShape r, float vx, float vy)
        : rect(r), vx(vx), vy(vy), alive(true), timeAlive(0.f) {
    }
    virtual void Update(float dt, sf::Vector2f cible = { 0.f, 0.f });
    virtual void Render(sf::RenderTarget* target) = 0;
    virtual ~Survival() = default;
};

enum class Pattern { Kamikaze };

class Ennemi : public Survival {
public:
    Pattern pattern;
    float timeAlive;
    float phaseX;
    float phaseY;
    int cycleX;
    sf::Vector2f center;
    bool hasSprite = false;

    enum class Direction {
        DOWN,
        LEFT,
        RIGHT,
        UP
    } state;

    Direction lastDirection = Direction::DOWN;
    Animator animator;

    Ennemi()
        : Survival(sf::RectangleShape(sf::Vector2f(60.f, 40.f)), 0.f, -1.f)
    {
        rect.setPosition({ 0.f, 0.f });
    }

    ~Ennemi();
    Ennemi(float x, float y, Pattern p);
    void ResolveCollisions(std::vector<Block*>& blocks);
    void Update(float dt, sf::Vector2f cible = { 0.f, 0.f }) override;
    void Render(sf::RenderTarget* target) override;
};

//PNJ(float x, float y, std::string spriteSheet)



class EnnemiSpawner {
public:
    EnnemiSpawner() = default;

    void Update(float dt, std::vector<Ennemi*>& ennemis, float spawnX, float spawnY);

private:
    int m_totalSpawned = 0;
    const int m_maxTotal = 100;

    float m_waveTimer = 0.f;     // temps entre deux vagues
    float m_spawnTimer = 0.f;    // temps entre deux ennemis dans une vague
    int   m_enemiesQueued = 0;   // ennemis restants à spawn dans la vague en cours

    float GetWaveInterval() const;
};