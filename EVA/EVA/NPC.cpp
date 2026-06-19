#include "NPC.h"
#include <algorithm>

NPC::NPC(float x, float y, std::vector<std::string> dialogues, std::vector<std::string> faces, sf::Font& font)
    : Entity(sf::RectangleShape(sf::Vector2f(32.f, 48.f)), 0.f, 0.f)
    , m_dialogues(dialogues)
    , m_faces(faces)
    , m_currentLine(0)
    , m_dialogueOpen(false)
    , m_font(font)
    , m_text(font, "", 60)
{
    rect.setPosition(sf::Vector2f(x, y));
    rect.setFillColor(sf::Color::Transparent);
    m_text.setFillColor(sf::Color::White);

    m_faceRect.setSize({ 250.f, 255.f });

    m_hasSound = m_dial.openFromFile("assets/sounds/talking.mp3");
    if (m_hasSound) {
        m_dial.setLooping(false);
        m_dial.setVolume(50.f);
    }

    m_hasDialogBoxTexture = m_dialogBoxTexture.loadFromFile("assets/pictures/dialogbox.png");
    if (!m_hasDialogBoxTexture) {
        m_dialogBox.setFillColor(sf::Color(20, 20, 20, 220));
    }
}

void NPC::SetupDialogBox(unsigned int winW, unsigned int winH) {
    if (m_boxInitialized) return;

    m_dialogBox.setPosition({ 0.f, static_cast<float>(winH) - 180.f });
    m_dialogBox.setSize({ static_cast<float>(winW), 180.f });
    if (m_hasDialogBoxTexture)
        m_dialogBox.setTexture(&m_dialogBoxTexture);

    m_faceRect.setPosition({ 50.f, static_cast<float>(winH) - 250.f });
    m_text.setPosition({ 300.f, static_cast<float>(winH) - 155.f });

    m_boxInitialized = true;
}

bool NPC::IsPlayerNear(sf::Vector2f playerPos, float range) {
    sf::Vector2f npcCenter = rect.getPosition() + sf::Vector2f(16.f, 24.f);
    float dx = playerPos.x - npcCenter.x;
    float dy = playerPos.y - npcCenter.y;
    return (dx * dx + dy * dy) <= (range * range);
}

void NPC::StartLine() {
    m_fullText = m_dialogues[m_currentLine];
    m_visibleChars = 0;
    m_charTimer = 0.f;
    m_isTyping = true;

    if (m_currentLine < (int)m_faces.size())
        m_faceTexture.loadFromFile(m_faces[m_currentLine]);
    m_faceRect.setTexture(&m_faceTexture);

    if (m_hasSound) m_dial.play();
}

void NPC::OpenDialogue() {
    m_currentLine = 0;
    m_dialogueOpen = true;
    StartLine();
}

bool NPC::NextDialogue() {
    m_currentLine++;
    if (m_currentLine >= (int)m_dialogues.size()) {
        CloseDialogue();
        return false;
    }
    StartLine();
    return true;
}

void NPC::CloseDialogue() {
    m_dialogueOpen = false;
    m_currentLine = 0;
    if (m_hasSound) m_dial.stop();
}

bool NPC::IsDialogueOpen() const {
    return m_dialogueOpen;
}

void NPC::UpdateTyping(float dt) {
    if (!m_dialogueOpen || !m_isTyping) return;

    m_charTimer += dt;
    if (m_charTimer >= m_charDelay) {
        m_charTimer = 0.f;
        m_visibleChars++;
        if (m_visibleChars >= (int)m_fullText.size()) {
            m_visibleChars = (int)m_fullText.size();
            m_isTyping = false;
            if (m_hasSound) m_dial.stop();
        }
    }
}

void NPC::Update(float dt, sf::Vector2u windowSize, std::vector<Block*>& blocks) {
    UpdateTyping(dt);
}

void NPC::RenderBubble(sf::RenderTarget* target, sf::RenderWindow& window, sf::View cameraView) {
    if (!m_dialogueOpen) return;

    SetupDialogBox(window.getSize().x, window.getSize().y);

    m_text.setString(m_fullText.substr(0, m_visibleChars));

    sf::View previousView = window.getView();
    window.setView(window.getDefaultView());
    window.draw(m_dialogBox);
    window.draw(m_faceRect);
    window.draw(m_text);
    window.setView(previousView);
}

void NPC::Render(sf::RenderTarget* target) {
    if (m_npcSprite)
        target->draw(*m_npcSprite);
}