#pragma once
#include <SFML/Graphics.hpp>

class Pause {
public:
    explicit Pause(sf::Font& font);

    void Render(sf::RenderWindow& window, const sf::View& menuView);
    void UpdateHover(sf::RenderWindow& window, const sf::View& menuView);

    bool IsResumeHovered() const { return m_resumeHovered; }
    bool IsQuitHovered() const { return m_quitHovered; }

private:
    sf::RectangleShape m_overlay;
    sf::RectangleShape m_resumeButton;
    sf::RectangleShape m_quitButton;
    sf::Text m_titleText;
    sf::Text m_resumeText;
    sf::Text m_quitText;
    bool m_resumeHovered = false;
    bool m_quitHovered = false;

    float GetResumeLeft() const { return m_resumeButton.getPosition().x; }
    float GetResumeRight() const { return m_resumeButton.getPosition().x + m_resumeButton.getSize().x; }
    float GetResumeTop() const { return m_resumeButton.getPosition().y; }
    float GetResumeBottom() const { return m_resumeButton.getPosition().y + m_resumeButton.getSize().y; }

    float GetQuitLeft() const { return m_quitButton.getPosition().x; }
    float GetQuitRight() const { return m_quitButton.getPosition().x + m_quitButton.getSize().x; }
    float GetQuitTop() const { return m_quitButton.getPosition().y; }
    float GetQuitBottom() const { return m_quitButton.getPosition().y + m_quitButton.getSize().y; }
};