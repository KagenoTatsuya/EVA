// CameraMenu.cpp
#include "CameraMenu.h"

CameraMenu::CameraMenu(float screenW, float screenH)
    : screenW(screenW), screenH(screenH) {
    menuview.setSize(sf::Vector2f(screenW, screenH));
    menuview.setCenter(sf::Vector2f(screenW / 2.f, screenH / 2.f));
}