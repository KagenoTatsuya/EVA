#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

#include "Entity.h"

class Camera {
private:
    sf::View view;
    float screenW, screenH;
    float levelW, levelH;

    float smoothSpeed = 5.f; // vitesse de lissage
    float deadzoneX = 80.f; // zone morte horizontale
    float deadzoneY = 50.f; // zone morte verticale

    /*
    smoothSpeed = 3.f;    // lent, tres cinematique
    smoothSpeed = 8.f;    // rapide, quasi immediat

    deadzoneX = 0.f;      // suit parfaitement le joueur
    deadzoneX = 150.f;    // grande zone morte, camera peu reactive
    */

public:

    Camera();
    Camera(float screenW, float screenH, float levelW, float levelH);

    void Update(float playerX, float playerY, float dt);

    sf::View GetView() const { return view; }

    ~Camera() {};
};