#include "SelectPerso.h"

PersoButton::PersoButton(float x, float y, float w, float h,
    const std::string& normalTexture, const std::string& hoverTexture,
    const std::string& spriteSheet)
    : posx(x), posy(y), width(w), height(h), m_spriteSheet(spriteSheet)
{
    LoadTextures(normalTexture, hoverTexture);
    button.setSize({ w, h });
    button.setPosition({ x, y });
    button.setTexture(&textureNormal);
}

void PersoButton::LoadTextures(const std::string& normal, const std::string& hover) {
    if (!textureNormal.loadFromFile(normal)) {
        std::cerr << "[ERREUR] Impossible de charger : " << normal << std::endl;
    }
    if (!textureHover.loadFromFile(hover)) {
        std::cerr << "[ERREUR] Impossible de charger : " << hover << std::endl;
    }
}

void PersoButton::Render(sf::RenderWindow& window) {
    window.draw(button);
}

void PersoButton::OnHover() {
    button.setTexture(&textureHover);

    float newW = width * m_hoverScale;
    float newH = height * m_hoverScale;

    // Recentre pour que l'agrandissement parte du centre du bouton
    float offsetX = (newW - width) / 10000.f;
    float offsetY = (newH - height) / 10000.f;

    button.setSize({ newW, newH });
    button.setPosition({ posx - offsetX, posy - offsetY });
}

void PersoButton::OnNormal() {
    button.setTexture(&textureNormal);
    button.setSize({ width, height });
    button.setPosition({ posx, posy });
}

SelectPerso::SelectPerso() {
    // Ajuste chemins de textures/portraits et positions selon tes assets réels.
    // Disposition en grille 3x2, à ajuster selon ta résolution de menu.
    const float startX = 635.f, startY = 300.f, w = 150.f, h = 200.f, gapX = 180.f, gapY = 230.f;

    struct PersoDef { std::string normal, hover, spritesheet; };
    std::array<PersoDef, kCount> defs = { {
        { "assets/pictures/BoyFace.png", "assets/pictures/BoyBody1.png", "character-spritesheet.png" },
        { "assets/pictures/BoyFace2.png", "assets/pictures/BoyBody2.png", "character-spritesheet6.png" },
        { "assets/pictures/BoyFace3.png", "assets/pictures/BoyBody3.png", "character-spritesheet7.png" },
        { "assets/pictures/BoyFace4.png", "assets/pictures/BoyBody4.png", "character-spritesheet11.png" },
        { "assets/pictures/GirlFace1.png", "assets/pictures/GirlBody1.png", "character-spritesheet10.png" },
        { "assets/pictures/GirlFace2.png", "assets/pictures/GirlBody2.png", "character-spritesheet8.png" },
        { "assets/pictures/GirlFace3.png", "assets/pictures/GirlBody3.png", "character-spritesheet9.png" },
        { "assets/pictures/GirlFace4.png", "assets/pictures/GirlBody4.png", "character-spritesheet12.png" },
    } };
    for (int i = 0; i < kCount; ++i) {
        float x = startX + (i % 4) * gapX;
        float y = startY + (i / 4) * gapY;
        m_buttons[i] = new PersoButton(x, y, w, h, defs[i].normal, defs[i].hover, defs[i].spritesheet);
    }
    if (!m_font.openFromFile("assets/fonts/arial.ttf"))
    {
        std::cerr << "[ERREUR] Impossible de charger la police pour SelectPerso" << std::endl;
    }

    m_instructionText.setFont(m_font);
    m_instructionText.setString("Choisis ton personnage");
    m_instructionText.setCharacterSize(50);
    m_instructionText.setFillColor(sf::Color::White);
    m_instructionText.setOutlineColor(sf::Color::Black);
    m_instructionText.setOutlineThickness(2.f);

    // Centre le texte horizontalement au-dessus de la grille de boutons
    sf::FloatRect bounds = m_instructionText.getLocalBounds();
    m_instructionText.setOrigin({ bounds.size.x / 2.f, 0.f });
    m_instructionText.setPosition({ startX + (2 * gapX), startY - 175.f }); // ajuste selon la mise en page
}

SelectPerso::~SelectPerso() {
    for (auto* b : m_buttons) delete b;
}

void SelectPerso::Render(sf::RenderWindow& window) {
    window.draw(m_instructionText);
    for (auto* b : m_buttons) b->Render(window);
}

void SelectPerso::Update(sf::RenderWindow& window, sf::View& menuView, std::vector<sf::Event>& events) {
    for (auto* b : m_buttons) b->UpdateHover(window, menuView);

    for (auto& ev : events) {
        if (auto* mb = ev.getIf<sf::Event::MouseButtonPressed>()) {
            if (mb->button == sf::Mouse::Button::Left) {
                for (int i = 0; i < kCount; ++i) {
                    if (m_buttons[i]->IsHovered()) {
                        m_selectedIndex = i;
                        m_confirmed = true;
                    }
                }
            }
        }
    }
}

std::string SelectPerso::GetSelectedSpriteSheet() const {
    if (m_selectedIndex < 0) return "character-spritesheet.png"; // fallback par défaut
    return m_buttons[m_selectedIndex]->GetSpriteSheet();
}

void SelectPerso::Reset() {
    m_selectedIndex = -1;
    m_confirmed = false;
}