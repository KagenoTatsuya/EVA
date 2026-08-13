#include "HUD.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>

HUD::HUD(sf::Font& f)
    : font(f), score(0), vies(3),
    scoreText(f, "", 40),
    viesText(f, "", 40)
{
}

// Update the score value
void HUD::setScore(int s) {
    score = s;
}

// Update the remaining lives
void HUD::setVies(int v) {
    vies = v;
}

// Render the HUD elements (score + lives)
void HUD::render(sf::RenderTarget& target) {
    renderText(target, "Score: " + std::to_string(score), 20.f, 20.f, sf::Color::White);
    renderText(target, "Vies: " + std::to_string(vies), 20.f, 50.f, sf::Color::White);
}

// Render a single line of text at a given position
void HUD::renderText(sf::RenderTarget& target, const std::string& message, float x, float y, sf::Color color) {
    sf::Text text(font, message, 40);
    text.setFillColor(color);
    text.setPosition(sf::Vector2f(x, y));
    target.draw(text);
}

void HUD::renderZoneGauge(sf::RenderTarget& target, const Zone& zone,
    sf::Vector2f position, sf::Vector2f size)
{
    float bleuPercent = std::clamp(zone.GetCapturePercent(Team::Bleu), 0.f, 100.f);
    float orangePercent = 100.f - bleuPercent;

    // Fond de la jauge (cadre)
    sf::RectangleShape background(size);
    background.setPosition(position);
    background.setFillColor(sf::Color(40, 40, 40, 200));
    background.setOutlineThickness(2.f);
    background.setOutlineColor(sf::Color::White);
    target.draw(background);

    // Hauteur remplie par le Bleu (en partant du HAUT de la jauge)
    float bleuHeight = size.y * (bleuPercent / 100.f);

    if (bleuHeight > 0.f) {
        sf::RectangleShape bleuFill({ size.x, bleuHeight });
        bleuFill.setPosition(position); // colle en haut
        bleuFill.setFillColor(sf::Color(60, 120, 255)); // bleu
        target.draw(bleuFill);
    }

    // Hauteur remplie par l'Orange (en partant du BAS de la jauge)
    float orangeHeight = size.y * (orangePercent / 100.f);

    if (orangeHeight > 0.f) {
        sf::RectangleShape orangeFill({ size.x, orangeHeight });
        orangeFill.setPosition({ position.x, position.y + size.y - orangeHeight }); // colle en bas
        orangeFill.setFillColor(sf::Color(255, 140, 0)); // orange
        target.draw(orangeFill);
    }

    // Ligne de séparation (utile pour bien voir la frontière de capture)
    sf::RectangleShape separator({ size.x, 2.f });
    separator.setPosition({ position.x, position.y + bleuHeight - 1.f });
    separator.setFillColor(sf::Color::White);
    target.draw(separator);

    // Label du nom de la zone au-dessus de la jauge
    sf::Text label(font, zone.displayName, 26);
    label.setFillColor(sf::Color::White);
    label.setOutlineColor(sf::Color::White);
    label.setOutlineThickness(0.5f);
    sf::FloatRect bounds = label.getLocalBounds();
    float labelMargin = 10.f;
    label.setPosition({position.x + size.x / 2.f - bounds.size.x / 2.f - bounds.position.x, position.y - bounds.size.y - bounds.position.y - labelMargin});
    target.draw(label);
}

// Affiche le timer de la partie, centré en haut de l'écran, format MM:SS
void HUD::renderTimer(sf::RenderTarget& target, float timeRemaining) {
    int totalSeconds = static_cast<int>(std::ceil(std::max(0.f, timeRemaining)));
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << minutes
        << ":" << std::setfill('0') << std::setw(2) << seconds;

    sf::Text timerText(font, oss.str(), 36);
    // Rouge quand il reste moins de 30 secondes, pour prévenir le joueur
    timerText.setFillColor(timeRemaining <= 30.f ? sf::Color::Red : sf::Color::White);

    sf::FloatRect bounds = timerText.getLocalBounds();
    float targetWidth = static_cast<float>(target.getSize().x);

    const float paddingX = 16.f;
    const float paddingY = 8.f;
    const float topMargin = 20.f;

    sf::Vector2f textPos(targetWidth / 2.f - bounds.size.x / 2.f, topMargin);

    // Fond gris derrière le texte, dimensionné sur les bounds réels du texte
    sf::RectangleShape background;
    background.setSize(sf::Vector2f(bounds.size.x + paddingX * 2.f, bounds.size.y + paddingY * 2.f));
    background.setFillColor(sf::Color(40, 40, 40, 200));
    background.setOutlineThickness(2.f);
    background.setOutlineColor(sf::Color::White);
    background.setPosition(sf::Vector2f(
        targetWidth / 2.f - background.getSize().x / 2.f,
        topMargin + paddingY
    ));

    timerText.setPosition(textPos);

    target.draw(background);
    target.draw(timerText);
}