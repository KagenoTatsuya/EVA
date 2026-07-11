#include "Shoot.h"
#include "Entity.h"
#include "Survival.h"


// Constructor: initializes a shot with position, velocity, and type
Shoot::Shoot(float x, float y, float vx, float vy, ShootType t) : vx(vx), vy(vy), alive(true), type(t) {
    this->vx = vx;
    this->vy = vy;
    rect.setPosition(sf::Vector2f(x, y));
    rect.setSize(sf::Vector2f(10.f, 8.f));

}

// Creates a thick laser shot (multiple parallel projectiles)
void Shoot::ShootEpais(std::vector<Shoot*>& shoot, const Joueur& player, int epaisseur, float spacing) {
    // Create multiple shots with vertical offset to simulate thickness
    for (int j = 0; j < epaisseur; ++j) {
        //Calculate vertical offset for this projectile
        float offsetY = (j - epaisseur / 2.0f) * spacing;

        //Position du joueur
        sf::Vector2f pos = player.rect.getPosition();
        sf::Vector2f size = player.rect.getSize();

        //Create projectile with vertical offset
        shoot.push_back(new Shoot(
            pos.x + size.x,                    // X: right side of player
            pos.y + size.y / 2.0f + offsetY,  // Y: centered with offset
            300.0f, 0.0f,                     // Velocity: horizontal only
            ShootType::Player)
        );
    }
}

void Shoot::handleInput(float dt, Joueur& player, std::vector<Shoot*>& shoot, float& shootCooldown, bool& specialActive, float& specialTimer, float& specialCooldown) {

    shootCooldown = std::max(0.0f, shootCooldown - dt);

    // Correction : vrai test de la touche Espace
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)
        && shootCooldown <= 0.f
        && !specialActive
        && specialCooldown <= 0.f)
    {
        specialActive = true;
        specialTimer = 0.1f;     // durée de l'attaque spéciale
        shootCooldown = 0.1f;    // délai avant le prochain tir
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

    // Position souris > coordonnées monde (tient compte du zoom/scroll caméra)
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel, view);

    // Vecteur direction normalisé
    sf::Vector2f dir = mouseWorld - playerCenter;
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length < 0.0001f) return; // évite division par zéro si souris == joueur

    dir /= length;

    shoot.push_back(new Shoot(
        playerCenter.x, playerCenter.y,
        dir.x * speed, dir.y * speed,
        ShootType::Player
    ));
}

void Shoot::Update(float dt, float now) {
    sf::Vector2f pos = rect.getPosition();
    pos.x += vx * dt;
    pos.y += vy * dt;
    rect.setPosition(pos);

    sf::Vector2f size = rect.getSize();
    if (pos.x + size.x < -1000 || pos.x > 4800.f ||
        pos.y + size.y < -1000 || pos.y > 3000.f) {
        alive = false;
    }
}

// Renders the shot with different colors based on type
void Shoot::Render(sf::RenderTarget& target) {
    if (!alive) return;
    if (type == ShootType::Player)
        rect.setFillColor(sf::Color::White);
    else
        rect.setFillColor(sf::Color::Red);
    target.draw(rect);
}