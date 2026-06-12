#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

class CameraMenu {
private:
    sf::View menuview;
    float screenW, screenH;

public:

    CameraMenu(float screenW, float screenH);
    sf::View GetMenuView() const { return menuview; }

    ~CameraMenu() {};
};