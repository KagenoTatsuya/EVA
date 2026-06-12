#include "Tile.h"

Tile::Tile(float x, float y, float width, float height, sf::Color color)
{
    m_shape.setSize({ width, height });
    m_shape.setFillColor(color);
    m_shape.setPosition({ x, y });
}

void Tile::Render(sf::RenderTarget* target)
{
    target->draw(m_shape);
}

sf::FloatRect Tile::getBounds() const
{
    return m_shape.getGlobalBounds();
}