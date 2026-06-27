#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Joueur;

// Origin of the shot
enum class ShootType {
    Player,
};

//Initialization Tir (Shoot)
class Shoot {
public:

    sf::RectangleShape rect;   // position and size
    float vx, vy;     // speed
    bool alive;       // state
    ShootType type;     // player or enemy
    int damage_tir = 75;

    // Manufacturer
    Shoot() = default;
    Shoot(float x, float y, float vx, float vy, ShootType t);

    static void handleInput(float dt, Joueur& player, std::vector<Shoot*>& shoot, float& shootCooldown, bool& specialActive, float& specialTimer, float& specialCooldown);

    // Update
    void Update(float dt, float now);
    void Render(sf::RenderTarget& target);

    // Utility functions
    //static void ShootCirculaire(std::vector<Shoot>& tirs, const Player& player);
    static void ShootEpais(std::vector<Shoot*>& tirs, const Joueur& player, int epaisseur, float spacing);

};