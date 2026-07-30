#include "TouchInput.h"
#include <cmath>

void TouchInput::ComputeLayoutIfNeeded(sf::Vector2u windowSize) {
    if (windowSize == m_lastWindowSize) return;
    m_lastWindowSize = windowSize;

    // Bouton de tir : bas droite, avec une marge
    m_shootButtonCenter = sf::Vector2f(
        static_cast<float>(windowSize.x) - 130.f,
        static_cast<float>(windowSize.y) - 130.f
    );
}

bool TouchInput::IsInCircle(sf::Vector2f point, sf::Vector2f center, float radius) {
    sf::Vector2f d = point - center;
    return (d.x * d.x + d.y * d.y) <= (radius * radius);
}

void TouchInput::Update(const std::vector<sf::Event>& events, sf::Vector2u windowSize) {
    if (!m_enabled) {
        m_moveVector = { 0.f, 0.f };
        m_shootHeld = false;
        return;
    }

    ComputeLayoutIfNeeded(windowSize);

    for (const auto& event : events) {
        if (const auto* began = event.getIf<sf::Event::TouchBegan>()) {
            sf::Vector2f pos(static_cast<float>(began->position.x), static_cast<float>(began->position.y));

            // Le bouton de tir est prioritaire (zone fixe, plus petite)
            if (!m_shootActive && IsInCircle(pos, m_shootButtonCenter, m_shootButtonRadius)) {
                m_shootActive = true;
                m_shootFingerId = began->finger;
                m_shootHeld = true;
            }
            // Sinon, toucher la moitié gauche de l'écran fait apparaître le joystick à cet endroit
            else if (!m_joystickActive && pos.x < static_cast<float>(windowSize.x) / 2.f) {
                m_joystickActive = true;
                m_joystickFingerId = began->finger;
                m_joystickBaseCenter = pos;
                m_joystickThumbPos = pos;
                m_moveVector = { 0.f, 0.f };
            }
        }
        else if (const auto* moved = event.getIf<sf::Event::TouchMoved>()) {
            sf::Vector2f pos(static_cast<float>(moved->position.x), static_cast<float>(moved->position.y));

            if (m_joystickActive && moved->finger == m_joystickFingerId) {
                sf::Vector2f delta = pos - m_joystickBaseCenter;
                float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);

                if (dist > m_joystickMaxRadius) {
                    delta = delta / dist * m_joystickMaxRadius;
                    dist = m_joystickMaxRadius;
                }

                m_joystickThumbPos = m_joystickBaseCenter + delta;
                m_moveVector = (m_joystickMaxRadius > 0.f) ? delta / m_joystickMaxRadius : sf::Vector2f{ 0.f, 0.f };
            }
        }
        else if (const auto* ended = event.getIf<sf::Event::TouchEnded>()) {
            if (m_joystickActive && ended->finger == m_joystickFingerId) {
                m_joystickActive = false;
                m_moveVector = { 0.f, 0.f };
            }
            if (m_shootActive && ended->finger == m_shootFingerId) {
                m_shootActive = false;
                m_shootHeld = false;
            }
        }
    }
}

void TouchInput::Render(sf::RenderWindow& window) const {
    if (!m_enabled) return;

    // --- Bouton de tir ---
    sf::CircleShape shootButton(m_shootButtonRadius);
    shootButton.setOrigin(sf::Vector2f(m_shootButtonRadius, m_shootButtonRadius));
    shootButton.setPosition(m_shootButtonCenter);
    shootButton.setFillColor(m_shootHeld ? sf::Color(255, 80, 80, 160) : sf::Color(255, 255, 255, 90));
    shootButton.setOutlineColor(sf::Color(255, 255, 255, 180));
    shootButton.setOutlineThickness(3.f);
    window.draw(shootButton);

    // --- Joystick (uniquement affiché tant qu'un doigt le contrôle) ---
    if (m_joystickActive) {
        sf::CircleShape base(m_joystickBaseRadius);
        base.setOrigin(sf::Vector2f(m_joystickBaseRadius, m_joystickBaseRadius));
        base.setPosition(m_joystickBaseCenter);
        base.setFillColor(sf::Color(255, 255, 255, 60));
        base.setOutlineColor(sf::Color(255, 255, 255, 150));
        base.setOutlineThickness(2.f);
        window.draw(base);

        sf::CircleShape thumb(m_joystickThumbRadius);
        thumb.setOrigin(sf::Vector2f(m_joystickThumbRadius, m_joystickThumbRadius));
        thumb.setPosition(m_joystickThumbPos);
        thumb.setFillColor(sf::Color(255, 255, 255, 180));
        window.draw(thumb);
    }
}