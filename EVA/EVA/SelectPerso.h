#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <string>
#include "Button.h"


class PersoButton : public Button {
    float posx, posy, width, height;
    std::string m_spriteSheet;
    std::string m_facePath;
    float m_hoverScale = 11.f;
public:
    PersoButton(float x, float y, float w, float h,
        const std::string& normalTexture, const std::string& hoverTexture,
        const std::string& spriteSheet);
    const std::string& GetSpriteSheet() const { return m_spriteSheet; }
    const std::string& GetFacePath() const { return m_facePath; }
    inline float GetPosX() override { return posx; }
    inline float GetRightX() override { return posx + width; }
    inline float GetPosY() override { return posy; }
    inline float GetBottomY() override { return posy + height; }
    void LoadTextures(const std::string& normal, const std::string& hover);
    void Render(sf::RenderWindow& window) override;
    void OnHover() override;
    void OnNormal() override;
};


class SelectPerso {
public:
    SelectPerso();
    ~SelectPerso();
    void Render(sf::RenderWindow& window);
    void Update(sf::RenderWindow& window, sf::View& menuView, std::vector<sf::Event>& events);
    bool IsConfirmed() const { return m_confirmed; }
    std::string GetSelectedSpriteSheet() const;
    std::string GetSelectedFacePath() const;
    void Reset();
private:
    static constexpr int kCount = 8;
    std::array<PersoButton*, kCount> m_buttons;
    int m_selectedIndex = -1;
    bool m_confirmed = false;
    sf::Font m_font;
    sf::Text m_instructionText{ m_font };
};