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

    float m_zoom = 0.5f;

    float m_boundsMinX = 0.f, m_boundsMinY = 0.f;
    float m_boundsMaxX = 0.f, m_boundsMaxY = 0.f;

    bool m_lockX = false;

    /*
    smoothSpeed = 3.f;    // lent, tres cinematique
    smoothSpeed = 8.f;    // rapide, quasi immediat

    deadzoneX = 0.f;      // suit parfaitement le joueur
    deadzoneX = 150.f;    // grande zone morte, camera peu reactive
    */
public:
    bool m_ceilingMode = false;

    Camera();
    Camera(float screenW, float screenH, float levelW, float levelH);

    void FitWholeLevel();
    void ResetZoom();
    void ForceCenter(sf::Vector2f pos);
    void Update(float playerX, float playerY, float dt);
    void SetCeilingMode(bool active);
    void SetZoom(float zoom);
    void SetViewSize(float width, float height);
    void SetLevelBounds(float minX, float minY, float maxX, float maxY);
    void SetAxisLockX(bool locked);

    sf::View GetView() const { return view; }

    ~Camera() {};    
};