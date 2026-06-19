#include "Camera.h"
#include <iostream>
#include <algorithm>
#include <cmath>


Camera::Camera() : Camera(0.0f, 0.0f, 0.0f, 0.0f) {
    std::cout << "default camera constructor";
}

Camera::Camera(float screenW, float screenH, float levelW, float levelH) : screenW(screenW), screenH(screenH), levelW(levelW), levelH(levelH) {
    //cam player
    view.setSize(sf::Vector2f(screenW, screenH));
    view.setCenter(sf::Vector2f(screenW / 2.f, screenH / 2.f));
}

void Camera::Update(float playerX, float playerY, float dt) {
    sf::Vector2f center = view.getCenter();

    if (m_ceilingMode) {
        // X suit le joueur, Y descend vers le haut PROGRESSIVEMENT
        float targetY = screenH / 2.f;
        float newX = center.x + (playerX - center.x) * smoothSpeed * dt;
        float newY = center.y + (targetY - center.y) * smoothSpeed * dt;

        newX = std::clamp(newX, screenW / 2.f, levelW);
        newY = std::clamp(newY, screenH / 2.f, levelH);
        view.setCenter(sf::Vector2f(newX, newY));
        return;
    }

    // ===== Comportement normal =====
    float targetX = center.x;
    float targetY = center.y;

    if (playerX > center.x + deadzoneX)
        targetX = playerX - deadzoneX;
    else if (playerX < center.x - deadzoneX)
        targetX = playerX + deadzoneX;

    if (playerY > center.y + deadzoneY)
        targetY = playerY - deadzoneY;
    else if (playerY < center.y - deadzoneY)
        targetY = playerY + deadzoneY;

    float newX = center.x + (targetX - center.x) * smoothSpeed * dt;
    float newY = center.y + (targetY - center.y) * smoothSpeed * dt;

    newX = std::clamp(newX, screenW / 2.f, levelW);
    newY = std::clamp(newY, screenH / 2.f, levelH);
    view.setCenter(sf::Vector2f(newX, newY));
}
void Camera::SetCeilingMode(bool active) {
    m_ceilingMode = active;
}