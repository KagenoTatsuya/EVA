#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Tile;

class Player2d
{
public:
    Player2d(float startX, float startY);

    void handleEvent(const sf::Event& event);
    void Update(float deltaTime, sf::Vector2u windowSize, const std::vector<Tile>& tiles);
    void Render(sf::RenderTarget* target);

    sf::FloatRect getBounds() const;
    sf::Vector2f  getPosition() const;

private:
    static constexpr float MOVE_SPEED = 250.f;
    static constexpr float JUMP_FORCE = -700.f;
    static constexpr float GRAVITY = 1500.f;
    static constexpr float JUMP_BUFFER_TIME = 0.12f;
    static constexpr float COYOTE_TIME = 0.10f;

    sf::RectangleShape m_shape;
    sf::Vector2f m_velocity;
    bool m_isOnGround;
    bool m_jumpRequested;
    float m_jumpBufferTimer;
    float m_coyoteTimer;
};