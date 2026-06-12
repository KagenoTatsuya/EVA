#pragma once
#include <SFML/Graphics.hpp>

class Tile
{
public:
    Tile(float x, float y, float width = 64.f, float height = 64.f,
        sf::Color color = sf::Color(100, 100, 100));

    void Render(sf::RenderTarget* target);

    sf::FloatRect getBounds() const;

private:
    sf::RectangleShape m_shape;
};