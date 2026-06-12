#pragma once
#include <SFML/Graphics.hpp>

class DarknessEffect {
public:
    DarknessEffect(sf::Vector2u windowSize, float radius = 150.f, float softness = 50.f);

    void setPlayerPos(sf::Vector2f pos);
    void setRadius(float radius);
    void setSoftness(float softness);

    void Render(sf::RenderWindow& window, const sf::RenderTexture& sceneTexture, sf::View cameraView);

private:
    sf::Shader      m_shader;
    sf::Vector2u    m_windowSize;
    float           m_radius;
    float           m_softness;
    sf::Vector2f    m_playerPos;
};