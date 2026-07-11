#ifndef HUD_H
#define HUD_H

#include "Zone.h"
#include "Battle.h"
#include <SFML/Graphics.hpp>
#include <string>

class HUD {
public:
    HUD(sf::Font& font);

    // Update of Values
    void setScore(int s);
    void setVies(int v);

    // Show the HUD
    void renderZoneGauge(sf::RenderTarget& target, const Zone& zone, sf::Vector2f position, sf::Vector2f size);
    void render(sf::RenderTarget& target);

private:
    sf::Font& font;
    int score;
    int vies;

    sf::Text scoreText;
    sf::Text viesText;

    // Internal utility function
    void renderText(sf::RenderTarget& target, const std::string& message, float x, float y, sf::Color color);
};

#endif // HUD_H