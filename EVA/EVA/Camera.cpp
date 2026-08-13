#include "Camera.h"
#include <iostream>
#include <algorithm>
#include <cmath>


Camera::Camera() : Camera(0.0f, 0.0f, 0.0f, 0.0f) {
    std::cout << "default camera constructor";
}

Camera::Camera(float screenW, float screenH, float levelW, float levelH)
    : screenW(screenW), screenH(screenH), levelW(levelW), levelH(levelH) {
    view.setSize(sf::Vector2f(screenW, screenH));
    view.setCenter(sf::Vector2f(screenW / 2.f, screenH / 2.f));
    // valeurs par défaut = comportement d'origine tant que SetLevelBounds n'est pas appelé
    m_boundsMinX = 0.f;
    m_boundsMinY = 0.f;
    m_boundsMaxX = levelW;
    m_boundsMaxY = levelH;
}

void Camera::Update(float playerX, float playerY, float dt) {
    sf::Vector2f center = view.getCenter();
    sf::Vector2f viewSize = view.getSize();
    float halfW = viewSize.x / 2.f;
    float halfH = viewSize.y / 2.f;
    float minCX = m_boundsMinX + halfW;
    float maxCX = std::max(minCX, m_boundsMaxX - halfW);
    float minCY = m_boundsMinY + halfH;
    float maxCY = std::max(minCY, m_boundsMaxY - halfH);

    if (m_ceilingMode) {
        float targetY = screenH / 2.f;
        float newX = m_lockX ? minCX : center.x + (playerX - center.x) * smoothSpeed * dt;
        float newY = center.y + (targetY - center.y) * smoothSpeed * dt;
        if (!m_lockX) newX = std::clamp(newX, minCX, maxCX);
        newY = std::clamp(newY, minCY, maxCY);
        view.setCenter(sf::Vector2f(newX, newY));
        return;
    }

    // ===== Comportement normal =====
    // Y : toujours calculé, verrou X ou pas
    float targetY = center.y;
    if (playerY > center.y + deadzoneY)
        targetY = playerY - deadzoneY;
    else if (playerY < center.y - deadzoneY)
        targetY = playerY + deadzoneY;
    float newY = center.y + (targetY - center.y) * smoothSpeed * dt;
    newY = std::clamp(newY, minCY, maxCY);

    // X : figé si m_lockX, sinon suivi normal
    float newX;
    if (m_lockX) {
        newX = minCX; // caméra fixe contre le bord gauche du niveau
    }
    else {
        float targetX = center.x;
        if (playerX > center.x + deadzoneX)
            targetX = playerX - deadzoneX;
        else if (playerX < center.x - deadzoneX)
            targetX = playerX + deadzoneX;
        newX = center.x + (targetX - center.x) * smoothSpeed * dt;
        newX = std::clamp(newX, minCX, maxCX);
    }

    view.setCenter(sf::Vector2f(newX, newY));
}

void Camera::SetCeilingMode(bool active) {
    m_ceilingMode = active;
}

void Camera::ForceCenter(sf::Vector2f pos) {
    view.setCenter(pos);
}

void Camera::FitWholeLevel() {
    // ratio nécessaire sur chaque axe pour que tout le niveau tienne à l'écran
    float scaleX = levelW / screenW;
    float scaleY = levelH / screenH;
    float scale = std::max(scaleX, scaleY); // on prend le plus contraignant des deux

    view.setSize(sf::Vector2f(screenW * scale, screenH * scale));
    view.setCenter(sf::Vector2f(levelW / 2.f, levelH / 2.f));
}

void Camera::ResetZoom() {
    view.setSize(sf::Vector2f(screenW, screenH));
}

void Camera::SetZoom(float zoom) {
    m_zoom = zoom;
    view.setSize(sf::Vector2f(screenW * zoom, screenH * zoom));
}
void Camera::SetViewSize(float width, float height) {
    screenW = width;
    screenH = height;
    m_zoom = 1.f;
    view.setSize(sf::Vector2f(screenW, screenH));
}
void Camera::SetLevelBounds(float minX, float minY, float maxX, float maxY) {
    m_boundsMinX = minX;
    m_boundsMinY = minY;
    m_boundsMaxX = maxX;
    m_boundsMaxY = maxY;
}

void Camera::SetAxisLockX(bool locked) {
    m_lockX = locked;
}