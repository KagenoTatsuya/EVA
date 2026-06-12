#include "Parallax.h"
#include <iostream>

Parallax::Parallax(float screenW, float screenH)
    : showInterior(false), screenW(screenW), screenH(screenH) {
}

void Parallax::Load(const std::string& exteriorPath) {
    if (!exterior.texture.loadFromFile(exteriorPath)) {
        std::cerr << "Erreur background extérieur: " << exteriorPath << std::endl;
    }
    exterior.texture.setRepeated(true);
    exterior.sprite1.emplace(exterior.texture);
    exterior.sprite2.emplace(exterior.texture);
    exterior.speedX = 0.3f;
    exterior.speedY = 0.1f;
}

void Parallax::Update(float cameraX, float cameraY) {
    auto updateLayer = [&](ParallaxLayer& layer) {
        float offsetX = fmod(cameraX * layer.speedX, screenW);
        float offsetY = fmod(cameraY * layer.speedY, screenH);
        layer.sprite1->setPosition(sf::Vector2f(-offsetX, offsetY + 200));
        layer.sprite2->setPosition(sf::Vector2f(screenW - offsetX, offsetY + 200));
        };
    updateLayer(exterior);
    if (showInterior) updateLayer(interior);
}

void Parallax::Render(sf::RenderWindow& window) {
    sf::View gameView = window.getView();
    sf::View bgView(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(screenW, screenH)));
    window.setView(bgView);

    if (showInterior) {
        if (interior.sprite1) window.draw(*interior.sprite1);
        if (interior.sprite2) window.draw(*interior.sprite2);
    }
    else {
        if (exterior.sprite1) window.draw(*exterior.sprite1);
        if (exterior.sprite2) window.draw(*exterior.sprite2);
    }

    window.setView(gameView);
}