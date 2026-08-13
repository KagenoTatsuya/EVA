#include "Pause.h"

Pause::Pause(sf::Font& font)
    : m_titleText(font, "PAUSE", 50),
    m_resumeText(font, "Reprendre", 34),
    m_quitText(font, "Quitter", 34)
{
    // Overlay semi-transparent sur toute la vue menu (résolution 1920x1080, cf CameraMenu)
    m_overlay.setSize(sf::Vector2f(1920.f, 1080.f));
    m_overlay.setPosition(sf::Vector2f(0.f, 0.f));
    m_overlay.setFillColor(sf::Color(0, 0, 0, 160));

    m_titleText.setFillColor(sf::Color::White);
    m_titleText.setPosition(sf::Vector2f(
        1920.f / 2.f - m_titleText.getLocalBounds().size.x / 2.f, 300.f));

    m_resumeButton.setSize(sf::Vector2f(300.f, 70.f));
    m_resumeButton.setPosition(sf::Vector2f(1920.f / 2.f - 150.f, 480.f));
    m_resumeButton.setFillColor(sf::Color(60, 60, 60));
    m_resumeButton.setOutlineThickness(2.f);
    m_resumeButton.setOutlineColor(sf::Color::White);

    m_resumeText.setFillColor(sf::Color::White);
    m_resumeText.setPosition(sf::Vector2f(
        m_resumeButton.getPosition().x + m_resumeButton.getSize().x / 2.f - m_resumeText.getLocalBounds().size.x / 2.f,
        m_resumeButton.getPosition().y + 15.f));

    m_quitButton.setSize(sf::Vector2f(300.f, 70.f));
    m_quitButton.setPosition(sf::Vector2f(1920.f / 2.f - 150.f, 580.f));
    m_quitButton.setFillColor(sf::Color(60, 60, 60));
    m_quitButton.setOutlineThickness(2.f);
    m_quitButton.setOutlineColor(sf::Color::White);

    m_quitText.setFillColor(sf::Color::White);
    m_quitText.setPosition(sf::Vector2f(
        m_quitButton.getPosition().x + m_quitButton.getSize().x / 2.f - m_quitText.getLocalBounds().size.x / 2.f,
        m_quitButton.getPosition().y + 15.f));
}

void Pause::UpdateHover(sf::RenderWindow& window, const sf::View& menuView) {
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel, menuView);

    m_resumeHovered = mouseWorld.x >= GetResumeLeft() && mouseWorld.x <= GetResumeRight() &&
        mouseWorld.y >= GetResumeTop() && mouseWorld.y <= GetResumeBottom();

    m_quitHovered = mouseWorld.x >= GetQuitLeft() && mouseWorld.x <= GetQuitRight() &&
        mouseWorld.y >= GetQuitTop() && mouseWorld.y <= GetQuitBottom();

    m_resumeButton.setFillColor(m_resumeHovered ? sf::Color(100, 100, 100) : sf::Color(60, 60, 60));
    m_quitButton.setFillColor(m_quitHovered ? sf::Color(100, 100, 100) : sf::Color(60, 60, 60));
}

void Pause::Render(sf::RenderWindow& window, const sf::View& menuView) {
    window.setView(menuView);
    window.draw(m_overlay);
    window.draw(m_titleText);
    window.draw(m_resumeButton);
    window.draw(m_resumeText);
    window.draw(m_quitButton);
    window.draw(m_quitText);
}