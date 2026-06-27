#pragma once
#include <SFML/Graphics.hpp>

class Battle {
public:
    sf::RectangleShape rect;
    float vx, vy;
    bool alive;
    float timeAlive;

    Battle(sf::RectangleShape r, float vx, float vy)
        : rect(r), vx(vx), vy(vy), alive(true), timeAlive(0.f) {
    }

    virtual void Update(float dt, sf::Vector2f cible = { 0.f, 0.f });
    virtual void Render(sf::RenderTarget* target) = 0; // méthode virtuelle pure
    virtual ~Battle() = default;
};

enum class Pattern { DiagonalBounce, Kamikaze };

class Soldat : public Battle {
public:
    Pattern pattern;
    float timeAlive;
    float phaseX;
    float phaseY;
    int cycleX;
    sf::Vector2f center;
    sf::Texture textureSoldat;
    sf::Sprite* spriteSoldat = nullptr;
    bool hasSprite = false;

    Soldat()
        : Battle(sf::RectangleShape(sf::Vector2f(60.f, 40.f)), 0.f, -1.f)
    {
        rect.setPosition({ 0.f, 0.f });
    }

    ~Soldat();

    Soldat(float x, float y, Pattern p);
    Soldat(float x, float y, Pattern p, float vitesse);

    void Update(float dt, sf::Vector2f cible = { 0.f, 0.f }) override;
    void Render(sf::RenderTarget* target) override;
};