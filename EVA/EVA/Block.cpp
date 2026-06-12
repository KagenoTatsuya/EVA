#include "Block.h"

Block::Block()
    : width(48.f), height(48.f), blockType("Block"),
    isNextLevel(false), isPrevLevel(false), isUse(false)
{
    rect = sf::RectangleShape(sf::Vector2f(width, height));
    rect.setFillColor(sf::Color::Transparent);
}

void Block::Render(sf::RenderTarget& target) {
    // DEBUG en rouge
    rect.setOutlineThickness(1.f);
    rect.setOutlineColor(sf::Color::Red);
    target.draw(rect);
}
MBlock::MBlock(float posx, float posy) {
    rect.setFillColor(sf::Color::Transparent);
    rect.setPosition(sf::Vector2f(posx, posy));
    blockType = "MBlock";
}

EndBlock::EndBlock(float posx, float posy) {
    width = 64.f;
    height = 64.f;
    rect.setSize(sf::Vector2f(width, height));
    rect.setFillColor(sf::Color::Transparent);
    //rect.setOutlineThickness(2.f);
    //rect.setOutlineColor(sf::Color::Yellow);
    rect.setPosition(sf::Vector2f(posx, posy));
    blockType = "EndBlock";
}

Item::Item(float posx, float posy) {
    rect.setFillColor(sf::Color::Transparent);
    //rect.setOutlineThickness(1.f);
    //rect.setOutlineColor(sf::Color::Green);
    rect.setPosition(sf::Vector2f(posx, posy));
    blockType = "Item";
}