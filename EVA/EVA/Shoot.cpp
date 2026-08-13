#include "Shoot.h"
#include "Entity.h"
#include "Survival.h"
#include <cmath>
#include <iostream>

// AJOUT : définition du membre statique (texture partagée par tous les tirs)
sf::Texture* Shoot::s_texture = nullptr;

// Constructor: initializes a shot with position, velocity, and type
Shoot::Shoot(float x, float y, float vx, float vy, ShootType t) : vx(vx), vy(vy), alive(true), type(t) {
    this->vx = vx;
    this->vy = vy;
    rect.setPosition(sf::Vector2f(x, y));
    rect.setSize(sf::Vector2f(10.f, 16.f));

    if (!s_texture) {
        s_texture = new sf::Texture("assets/pictures/Ball.png");
    }

    sprite = new sf::Sprite(*s_texture);
    sf::Vector2u texSize = s_texture->getSize();
    if (texSize.x > 0 && texSize.y > 0) {
        sprite->setOrigin(sf::Vector2f(texSize.x / 2.f, texSize.y / 2.f));
        sprite->setScale(sf::Vector2f(rect.getSize().x / texSize.x, rect.getSize().y / texSize.y));
    }

    // Le bout orange est en HAUT de l'image (pas à droite) : +90° pour aligner
    // "haut de l'image" avec la direction (vx, vy) du tir.
    float angleDeg = std::atan2(vy, vx) * 180.f / 3.14159265f + 90.f;
    sprite->setRotation(sf::degrees(angleDeg));
    sprite->setPosition(rect.getPosition() + rect.getSize() / 2.f);
}

Shoot::~Shoot() {
    delete sprite;
    sprite = nullptr;
}

// Creates a thick laser shot (multiple parallel projectiles)
void Shoot::ShootEpais(std::vector<Shoot*>& shoot, const Joueur& player, int epaisseur, float spacing) {
    for (int j = 0; j < epaisseur; ++j) {
        float offsetY = (j - epaisseur / 2.0f) * spacing;
        sf::Vector2f pos = player.rect.getPosition();
        sf::Vector2f size = player.rect.getSize();
        shoot.push_back(new Shoot(
            pos.x + size.x,
            pos.y + size.y / 2.0f + offsetY,
            300.0f, 0.0f,
            ShootType::Player)
        );
    }
}

void Shoot::handleInput(float dt, Joueur& player, std::vector<Shoot*>& shoot, float& shootCooldown, bool& specialActive, float& specialTimer, float& specialCooldown) {

    shootCooldown = std::max(0.0f, shootCooldown - dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)
        && shootCooldown <= 0.f
        && !specialActive
        && specialCooldown <= 0.f)
    {
        specialActive = true;
        specialTimer = 0.1f;
        shootCooldown = 0.1f;
    }

    if (specialTimer > 0.0f) {
        specialTimer -= dt;
        if (specialTimer <= 0.0f) {
            specialActive = false;
            specialCooldown = 1.0f;
        }
    }

    if (specialCooldown > 0.0f) {
        specialCooldown -= dt;
    }

    if (specialActive) {
        const int thickness = 1;
        const float spacing = 2.f;
        for (int j = 0; j < thickness; ++j) {
            float offsetY = (j - thickness / 2.0f) * spacing;
            sf::Vector2f pos = player.rect.getPosition();
            sf::Vector2f size = player.rect.getSize();
            shoot.push_back(new Shoot(
                pos.x + size.x,
                pos.y + size.y / 2.0f + offsetY,
                300.0f, 0.0f,
                ShootType::Player)
            );
        }
    }
}

void Shoot::ShootVersSouris(std::vector<Shoot*>& shoot, const Entity& player,
    sf::RenderWindow& window, sf::View& view, float speed)
{
    sf::Vector2f pos = player.rect.getPosition();
    sf::Vector2f size = player.rect.getSize();
    sf::Vector2f playerCenter = pos + size / 2.f;

    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel, view);

    sf::Vector2f dir = mouseWorld - playerCenter;
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length < 0.0001f) return;

    dir /= length;

    shoot.push_back(new Shoot(
        playerCenter.x, playerCenter.y,
        dir.x * speed, dir.y * speed,
        ShootType::Player
    ));
}

void Shoot::Update(float dt, float now, const std::vector<Block*>& blocks) {
    sf::Vector2f pos = rect.getPosition();
    pos.x += vx * dt;
    pos.y += vy * dt;
    rect.setPosition(pos);

    // AJOUT : le sprite suit la hitbox
    if (sprite) {
        sprite->setPosition(pos + rect.getSize() / 2.f);
    }

    sf::Vector2f size = rect.getSize();
    if (pos.x + size.x < -1000 || pos.x > 4800.f ||
        pos.y + size.y < -1000 || pos.y > 3000.f) {
        alive = false;
        return;
    }

    sf::FloatRect shootBounds = rect.getGlobalBounds();
    for (Block* b : blocks) {
        if (b->GetBlockType() == "MBlock" &&
            shootBounds.findIntersection(b->rect.getGlobalBounds())) {
            alive = false;
            break;
        }
    }
}

// Renders the shot using its sprite (image) instead of a plain rectangle
void Shoot::Render(sf::RenderTarget& target) {
    if (!alive) return;
    if (sprite) {
        target.draw(*sprite);
    }
}