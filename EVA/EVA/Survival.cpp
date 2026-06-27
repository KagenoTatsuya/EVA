#include "Survival.h"
#include "Deplacement.h"
#include <cmath>
#include <numbers>

Ennemi::Ennemi(float x, float y, Pattern p)
    : Survival(sf::RectangleShape(sf::Vector2f(60.f, 40.f)), 0.f, -1.f)
    , pattern(p)
    , timeAlive(0.f)
    , phaseX(0.f)
    , phaseY(0.f)
    , cycleX(0)
    , center{ 720.f, y }
{
    rect.setPosition({ x, y });

    hasSprite = textureEnnemi.loadFromFile("character-spritesheetZ.png", "character-spritesheetS.png");
    if (hasSprite) {
        spriteEnnemi = new sf::Sprite(textureEnnemi);
    }
}

Ennemi::Ennemi(float x, float y, Pattern p, float vitesse)
    : Survival(sf::RectangleShape(sf::Vector2f(60.f, 40.f)), -vitesse * 0.5f, -vitesse)
    , pattern(p)
    , timeAlive(0.f)
    , phaseX(0.f)
    , phaseY(0.f)
    , cycleX(0)
    , center{ 720.f, y }
{
    rect.setPosition({ x, y });
}

void Survival::Update(float dt, sf::Vector2f cible) {
    // comportement par défaut, ou vide
}

void Ennemi::Update(float dt, sf::Vector2f cible) {
    timeAlive += dt;

    const float minX = 350.f;
    const float maxX = 1440.f;
    const float centerX = (minX + maxX) / 2.f;
    const float amplitudeX = (maxX - minX) / 2.f;

    const float entryDuration = 12.f;
    bool isEntering = (timeAlive < entryDuration);

    sf::Vector2f pos = rect.getPosition();
    sf::Vector2f size = rect.getSize();

    switch (pattern) {

    case Pattern::DiagonalBounce: {
        const float horizontalFrequency = 0.8f;
        pos.x = centerX + std::cos(timeAlive * horizontalFrequency) * amplitudeX;

        pos.y += vy * 150.f * dt;
        if (pos.y <= 0.f || pos.y + size.y >= 900.f) {
            vy = -vy;
        }
        break;
    }

    case Pattern::Kamikaze: {
        sf::Vector2f depart = pos;
        sf::Vector2f arrivee = Deplacement::getPointArrive(
            depart, cible, 100.f * dt, 1440.f, 900.f
        );
        pos = arrivee;
        break;
    }
    }

    if (timeAlive >= 3.f) {
        pos.x = std::max(0.f, std::min(pos.x, 1440.f - size.x));
        pos.y = std::max(0.f, std::min(pos.y, 900.f - size.y));
    }

    rect.setPosition(pos);
}

void Ennemi::Render(sf::RenderTarget* target) {
    if (hasSprite && spriteEnnemi) {
        spriteEnnemi->setPosition(rect.getPosition());
        target->draw(*spriteEnnemi);
    }
    else {
        rect.setFillColor(sf::Color::Red);
        target->draw(rect);
    }
}

Ennemi::~Ennemi() {
    delete spriteEnnemi;
}