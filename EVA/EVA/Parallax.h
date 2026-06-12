#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <optional>

struct ParallaxLayer {
    sf::Texture texture;
    std::optional<sf::Sprite> sprite1;
    std::optional<sf::Sprite> sprite2;
    float speedX = 0.f;
    float speedY = 0.f;
};

class Parallax {
    ParallaxLayer exterior;
    ParallaxLayer interior;
    bool showInterior;
    float screenW;
    float screenH;

public:
    Parallax(float screenW, float screenH);
    void Load(const std::string& exteriorPath);
    void Update(float cameraX, float cameraY);
    void Render(sf::RenderWindow& window);
    void SetShowInterior(bool show);
};