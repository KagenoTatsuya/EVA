#include "NPC.h"
#include <algorithm>

NPC::NPC(float x, float y, std::vector<std::string> dialogues, sf::Font& font)
    : Entity(sf::RectangleShape(sf::Vector2f(32.f, 48.f)), 0.f, 0.f)
    , m_dialogues(dialogues)
    , m_currentLine(0)
    , m_dialogueOpen(false)
    , m_font(font)
    , m_text(font, "", 16)
{
    rect.setPosition(sf::Vector2f(x, y));
    rect.setFillColor(sf::Color::Transparent);

    // Visuel PNJ (rectangle coloré, remplace par un sprite si tu en as un)
    //m_visual.setSize(sf::Vector2f(32.f, 48.f));
    //m_visual.setFillColor(sf::Color(100, 180, 255));
    //m_visual.setPosition(sf::Vector2f(x, y));

    /*if (m_npcTexture.loadFromFile("ketchup.png")) {
        m_npcSprite = new sf::Sprite(m_npcTexture);
        m_npcSprite->setPosition(sf::Vector2f(x, y));
        // Ajuste la scale si le sprite est trop grand/petit
        m_npcSprite->setScale(sf::Vector2f(0.5f, 0.5f));
    }*/

    // Bulle de dialogue
    m_bubble.setFillColor(sf::Color(240, 240, 240, 230));
    m_bubble.setOutlineColor(sf::Color(60, 60, 60));
    m_bubble.setOutlineThickness(2.f);

    // Queue de la bulle
    m_tail.setSize(sf::Vector2f(12.f, 12.f));
    m_tail.setFillColor(sf::Color(240, 240, 240, 230));
    m_tail.setOutlineColor(sf::Color(60, 60, 60));
    m_tail.setOutlineThickness(2.f);
    m_tail.setRotation(sf::degrees(45.f));

    // Texte
    m_text.setFillColor(sf::Color(30, 30, 30));
    m_text.setCharacterSize(16);
}

bool NPC::IsPlayerNear(sf::Vector2f playerPos, float range) {
    sf::Vector2f npcCenter = rect.getPosition() + sf::Vector2f(16.f, 24.f);
    float dx = playerPos.x - npcCenter.x;
    float dy = playerPos.y - npcCenter.y;
    return (dx * dx + dy * dy) <= (range * range);
}

void NPC::OpenDialogue() {
    m_currentLine = 0;
    m_dialogueOpen = true;
    BuildBubble();
}

void NPC::CloseDialogue() {
    m_dialogueOpen = false;
    m_currentLine = 0;
}

bool NPC::IsDialogueOpen() const {
    return m_dialogueOpen;
}

bool NPC::NextDialogue() {
    m_currentLine++;
    if (m_currentLine >= (int)m_dialogues.size()) {
        CloseDialogue();
        return false;
    }
    BuildBubble(); // rebuild avec la nouvelle ligne
    return true;
}

void NPC::RenderBubble(sf::RenderTarget* target, sf::RenderWindow& window, sf::View cameraView) {
    if (!m_dialogueOpen) return;

    sf::Vector2f npcPos = rect.getPosition() + sf::Vector2f(16.f, 0.f);
    sf::Vector2i screenPos = window.mapCoordsToPixel(npcPos, cameraView);

    m_text.setString(m_dialogues[m_currentLine]);
    float textWidth = m_text.getLocalBounds().size.x;
    float textHeight = m_text.getLocalBounds().size.y;
    float padding = 16.f;
    float bubbleW = std::max(textWidth + padding * 2.f, 80.f);
    float bubbleH = textHeight + padding * 2.f + 10.f;

    float bubbleX = static_cast<float>(screenPos.x) - bubbleW / 2.f;
    float bubbleY = static_cast<float>(screenPos.y) - bubbleH - 20.f;

    m_bubble.setSize(sf::Vector2f(bubbleW, bubbleH));
    m_bubble.setPosition(sf::Vector2f(bubbleX, bubbleY));

    m_text.setPosition(sf::Vector2f(bubbleX + padding, bubbleY + padding / 2.f));

    m_tail.setPosition(sf::Vector2f(
        bubbleX + bubbleW / 2.f - 6.f,
        bubbleY + bubbleH - 8.f
    ));

    sf::View previousView = window.getView();
    window.setView(window.getDefaultView());
    window.draw(m_bubble);
    window.draw(m_tail);
    window.draw(m_text);
    window.setView(previousView);
}

void NPC::BuildBubble() {
    if (m_dialogues.empty()) return;

    std::string line = m_dialogues[m_currentLine];

    // Calcul largeur dynamique selon le texte
    m_text.setString(line);
    float textWidth = m_text.getLocalBounds().size.x;
    float textHeight = m_text.getLocalBounds().size.y;

    float padding = 16.f;
    float bubbleW = std::max(textWidth + padding * 2.f, 80.f);
    float bubbleH = textHeight + padding * 2.f + 10.f;

    sf::Vector2f npcPos = rect.getPosition();

    // Bulle au-dessus du PNJ, centrée
    float bubbleX = npcPos.x + 16.f - bubbleW / 2.f;
    float bubbleY = npcPos.y - bubbleH - 20.f;

    m_bubble.setSize(sf::Vector2f(bubbleW, bubbleH));
    m_bubble.setPosition(sf::Vector2f(bubbleX, bubbleY));

    // Texte centré dans la bulle
    m_text.setPosition(sf::Vector2f(
        bubbleX + padding,
        bubbleY + padding / 2.f
    ));

    // Queue centrée sous la bulle
    m_tail.setPosition(sf::Vector2f(
        bubbleX + bubbleW / 2.f - 6.f,
        bubbleY + bubbleH - 8.f
    ));
}

void NPC::Update(float dt, sf::Vector2u windowSize, std::vector<Block*>& blocks) {
    // Le PNJ ne bouge pas, rien à mettre à jour
}

void NPC::Render(sf::RenderTarget* target) {
    if (m_npcSprite)
        target->draw(*m_npcSprite);

    /*if (m_dialogueOpen) {
        target->draw(m_bubble);
        target->draw(m_tail);
        target->draw(m_text);
    }*/
}

