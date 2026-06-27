#include "NPC.h"
#include <algorithm>

NPC::NPC(float x, float y, std::vector<std::string> dialogues, std::vector<std::string> faces,
    std::vector<int> speakers, const std::string& playerFacePath, sf::Font& font)
    : Entity(sf::RectangleShape(sf::Vector2f(32.f, 48.f)), 0.f, 0.f)
    , m_dialogues(dialogues)
    , m_faces(faces)
    , m_speakers(speakers)
    , m_currentLine(0)
    , m_dialogueOpen(false)
    , m_font(font)
    , m_text(font, "", 60)
{
    rect.setPosition(sf::Vector2f(x, y));
    rect.setFillColor(sf::Color::Transparent);
    m_text.setFillColor(sf::Color::White);

    m_faceRect.setSize({ 250.f, 255.f });
    m_playerFaceRect.setSize({ 250.f, 255.f });

    // Portrait du joueur, fixe, chargé une seule fois
    m_hasPlayerFace = m_playerFaceTexture.loadFromFile(playerFacePath);
    if (m_hasPlayerFace)
        m_playerFaceRect.setTexture(&m_playerFaceTexture);

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

    // Portrait joueur à gauche
    m_playerFaceRect.setPosition({ 50.f, static_cast<float>(winH) - 250.f });

    // Portrait NPC à droite
    m_faceRect.setPosition({ static_cast<float>(winW) - 300.f, static_cast<float>(winH) - 250.f });

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

    int side = (m_currentLine < (int)m_speakers.size()) ? m_speakers[m_currentLine] : 0;

    if (side == 1) {
        // C'est le NPC (barman) qui parle -> charge son portrait
        if (m_currentLine < (int)m_faces.size() && !m_faces[m_currentLine].empty())
            m_faceTexture.loadFromFile(m_faces[m_currentLine]);
        m_faceRect.setTexture(&m_faceTexture);
    }

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

bool NPC::IsLastDialogue() const {
    return m_currentLine >= (int)m_dialogues.size() - 1;
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

void NPC::Update(float dt, sf::Vector2u windowSize, std::vector<Block*>& blocks, std::vector<Shoot*>& shoot, bool canShoot) {
    UpdateTyping(dt);
}

void NPC::RenderBubble(sf::RenderTarget* target, sf::RenderWindow& window, sf::View cameraView) {
    if (!m_dialogueOpen) return;

    SetupDialogBox(window.getSize().x, window.getSize().y);

    m_text.setString(m_fullText.substr(0, m_visibleChars));

    int side = (m_currentLine < (int)m_speakers.size()) ? m_speakers[m_currentLine] : 0;

    sf::View previousView = window.getView();
    window.setView(window.getDefaultView());
    window.draw(m_dialogBox);

    if (side == 0 && m_hasPlayerFace)
        window.draw(m_playerFaceRect);   // ligne du joueur -> portrait à gauche
    else
        window.draw(m_faceRect);         // ligne du NPC -> portrait à droite

    m_text.setPosition(side == 0
        ? sf::Vector2f(330.f, static_cast<float>(window.getSize().y) - 155.f)
        : sf::Vector2f(60.f, static_cast<float>(window.getSize().y) - 155.f));

    window.draw(m_text);
    window.setView(previousView);
}

void NPC::Render(sf::RenderTarget* target) {
    if (m_npcSprite)
        target->draw(*m_npcSprite);
}

// Choix ==================================================================================================

ChoiceDialog::ChoiceDialog() {
    font.openFromFile("font/Pixellettersfull-BnJ5.ttf");

    prompt = new sf::Text(font, "A quel jeu veux tu jouer ?", 70);
    lblPlay = new sf::Text(font, "Zombie", 60);
    lblWork = new sf::Text(font, "Battle", 60);

    prompt->setFillColor(sf::Color::White);
    lblPlay->setFillColor(sf::Color::White);
    lblWork->setFillColor(sf::Color::White);
}

void ChoiceDialog::init(unsigned int winW, unsigned int winH) {
    const float panelW = 600, panelH = 250;
    const float panelX = (winW - panelW) / 2;
    const float panelY = winH - panelH - 20;

    panel.setSize({ panelW, panelH });
    panel.setPosition({ panelX, panelY });

    texturePanel.loadFromFile("sprite/dialogbox.png");
    panel.setTexture(&texturePanel);

    prompt->setPosition({ panelX + 30, panelY + 20 });

    const float btnW = 240, btnH = 70;
    const float btnY = panelY + 120;

    buttonPlay.setSize({ btnW, btnH });
    buttonPlay.setPosition({ panelX + 40, btnY });
    textureButtonPlay.loadFromFile("sprite/button.png");
    buttonPlay.setTexture(&textureButtonPlay);

    buttonWork.setSize({ btnW, btnH });
    buttonWork.setPosition({ panelX + panelW - 40 - btnW, btnY });
    textureButtonWork.loadFromFile("sprite/button.png");
    buttonWork.setTexture(&textureButtonWork);

    lblPlay->setPosition({ buttonPlay.getPosition().x + 40, buttonPlay.getPosition().y });
    lblWork->setPosition({ buttonWork.getPosition().x + 60, buttonWork.getPosition().y });

    playBounds = buttonPlay.getGlobalBounds();
    workBounds = buttonWork.getGlobalBounds();
}

bool ChoiceDialog::isVisible() const {
    return visible;
}

void ChoiceDialog::show() {
    visible = true;
    chosen = Choice::None;
    prevPressed = true;
}

void ChoiceDialog::hide() {
    visible = false;
}

ChoiceDialog::Choice ChoiceDialog::getChoice() const {
    return chosen;
}

ChoiceDialog::Choice ChoiceDialog::update(const sf::RenderWindow& window) {
    if (!visible) {
        return Choice::None;
    }

    bool curPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    if (curPressed && !prevPressed) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos, window.getDefaultView());

        if (playBounds.contains(mousePos)) {
            chosen = Choice::PlayALittle;
            visible = false;
            prevPressed = curPressed;
            return chosen;
        }
        if (workBounds.contains(mousePos)) {
            chosen = Choice::Work;
            visible = false;
            prevPressed = curPressed;
            return chosen;
        }
    }

    prevPressed = curPressed;
    return Choice::None;
}

void ChoiceDialog::render(sf::RenderWindow& window) {
    if (!visible) {
        return;
    }
    window.draw(panel);
    window.draw(*prompt);
    window.draw(buttonPlay);
    window.draw(buttonWork);
    window.draw(*lblPlay);
    window.draw(*lblWork);
}