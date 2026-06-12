#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <algorithm>
#include "Animator.h"
#include "time.h"
#include "Block.h"

// ===== Entity class =====
class Entity {
public:
    sf::RectangleShape rect;
    float vx, vy;
    float width;
    float height;
    float posx;
    float posy;
    bool alive;

    // Manufacturer
    Entity(sf::RectangleShape r, float vx, float vy);

    // Virtual methods
    virtual void Update(float dt, sf::Vector2u windowSize, std::vector<Block*>& blocks);
    virtual void Render(sf::RenderTarget* target) = 0; // pure virtual method
    virtual ~Entity() = default; // virtual destructor to prevent leaks
};

// ===== Player =====
class Joueur : public Entity {
public:

    enum class Direction {
        DOWN,
        LEFT,
        RIGHT,
        UP
    } state;

    Direction lastDirection = Direction::DOWN; // par defaut

    Animator animator;
    Joueur(float x, float y);
    sf::FloatRect GetGlobalBounds() const;
    void Update(float dt, sf::Vector2u windowSize, std::vector<Block*>& blocks) override; // player movement
    void Render(sf::RenderTarget* target) override;
    void ResolveCollisions(std::vector<Block*>& blocks);
};