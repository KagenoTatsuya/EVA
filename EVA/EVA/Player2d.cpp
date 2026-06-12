#include <iostream>
#include "Player2d.h"
#include "Tile.h"

Player2d::Player2d(float startX, float startY)
    : m_velocity(0.f, 0.f)
    , m_isOnGround(false)
    , m_jumpRequested(false)
    , m_jumpBufferTimer(0.f)
    , m_coyoteTimer(0.f)
{
    m_shape.setSize({ 50.f, 50.f });
    m_shape.setFillColor(sf::Color::Cyan);
    m_shape.setOrigin({ 25.f, 50.f });
    m_shape.setPosition({ startX, startY });
}

void Player2d::handleEvent(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->code == sf::Keyboard::Key::Space ||
            keyPressed->code == sf::Keyboard::Key::Up)
        {
            m_jumpRequested = true;
            m_jumpBufferTimer = JUMP_BUFFER_TIME;
        }
    }
}

void Player2d::Update(float deltaTime, sf::Vector2u windowSize, const std::vector<Tile>& tiles)
{
    if (m_jumpBufferTimer > 0.f) m_jumpBufferTimer -= deltaTime;
    if (m_isOnGround)            m_coyoteTimer = COYOTE_TIME;
    else if (m_coyoteTimer > 0.f) m_coyoteTimer -= deltaTime;

    bool canJump = m_isOnGround || m_coyoteTimer > 0.f;
    if (m_jumpBufferTimer > 0.f && canJump)
    {
        m_velocity.y = JUMP_FORCE;
        m_isOnGround = false;
        m_jumpRequested = false;
        m_jumpBufferTimer = 0.f;
        m_coyoteTimer = 0.f;
    }

    m_velocity.x = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
        m_velocity.x = -MOVE_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        m_velocity.x = MOVE_SPEED;

    if (!m_isOnGround)
        m_velocity.y += GRAVITY * deltaTime;

    m_shape.move(m_velocity * deltaTime);

    m_isOnGround = false;

    for (const auto& tile : tiles)
    {
        sf::FloatRect playerBounds = m_shape.getGlobalBounds();
        sf::FloatRect tileBounds = tile.getBounds();

        if (playerBounds.findIntersection(tileBounds))
        {
            float overlapLeft = (playerBounds.position.x + playerBounds.size.x) - tileBounds.position.x;
            float overlapRight = (tileBounds.position.x + tileBounds.size.x) - playerBounds.position.x;
            float overlapTop = (playerBounds.position.y + playerBounds.size.y) - tileBounds.position.y;
            float overlapBottom = (tileBounds.position.y + tileBounds.size.y) - playerBounds.position.y;

            bool fromLeft = overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom;
            bool fromRight = overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom;
            bool fromTop = overlapTop < overlapBottom && overlapTop < overlapLeft && overlapTop < overlapRight;
            bool fromBottom = overlapBottom < overlapTop && overlapBottom < overlapLeft && overlapBottom < overlapRight;

            if (fromTop && m_velocity.y > 0.f)
            {
                m_shape.setPosition({ m_shape.getPosition().x, tileBounds.position.y });
                m_velocity.y = 0.f;
                m_isOnGround = true;
            }
            else if (fromBottom && m_velocity.y < 0.f)
            {
                m_shape.setPosition({ m_shape.getPosition().x,
                                      tileBounds.position.y + tileBounds.size.y + m_shape.getSize().y });
                m_velocity.y = 0.f;
            }
            else if (fromLeft && m_velocity.x > 0.f)
            {
                m_shape.setPosition({ tileBounds.position.x - m_shape.getSize().x / 2.f,
                                      m_shape.getPosition().y });
                m_velocity.x = 0.f;
            }
            else if (fromRight && m_velocity.x < 0.f)
            {
                m_shape.setPosition({ tileBounds.position.x + tileBounds.size.x + m_shape.getSize().x / 2.f,
                                      m_shape.getPosition().y });
                m_velocity.x = 0.f;
            }
        }
    }

    sf::Vector2f pos = m_shape.getPosition();
    if (pos.y >= 700.f)
    {
        m_shape.setPosition({ 50.f, 550.f });
    }
}

void Player2d::Render(sf::RenderTarget* target)
{
    if (!target) {
        std::cout << "ERREUR - target nullptr dans Player2d::Render" << std::endl;
        return;
    }
    target->draw(m_shape);
}

sf::FloatRect Player2d::getBounds() const
{
    return m_shape.getGlobalBounds();
}

sf::Vector2f Player2d::getPosition() const
{
    return m_shape.getPosition();
}