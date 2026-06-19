#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include "Entity.h"

class NPC : public Entity {
public:
    NPC(float x, float y, std::vector<std::string> dialogues, std::vector<std::string> faces, sf::Font& font);
    void RenderBubble(sf::RenderTarget* target, sf::RenderWindow& window, sf::View cameraView);
    void Update(float dt, sf::Vector2u windowSize, std::vector<Block*>& blocks) override;
    void Render(sf::RenderTarget* target) override;

    bool IsPlayerNear(sf::Vector2f playerPos, float range = 100.f);
    bool NextDialogue();
    void OpenDialogue();
    void CloseDialogue();
    bool IsDialogueOpen() const;

private:
    void SetupDialogBox(unsigned int winW, unsigned int winH);
    void StartLine();
    void UpdateTyping(float dt);

    std::vector<std::string>    m_dialogues;
    std::vector<std::string>    m_faces;
    int                         m_currentLine;
    bool                        m_dialogueOpen;

    sf::Texture m_npcTexture;
    sf::Sprite* m_npcSprite = nullptr;
    sf::Font& m_font;

    // Bandeau de dialogue (fixe, façon Dialog)
    sf::RectangleShape          m_dialogBox;
    sf::Texture                 m_dialogBoxTexture;
    bool                        m_hasDialogBoxTexture = false;

    sf::Text                    m_text;

    // Portrait
    sf::Texture                 m_faceTexture;
    sf::RectangleShape          m_faceRect;

    // Effet de frappe
    std::string                 m_fullText;
    int                         m_visibleChars = 0;
    float                       m_charTimer = 0.f;
    float                       m_charDelay = 0.05f;
    bool                        m_isTyping = false;

    // Son
    sf::Music                   m_dial;
    bool                        m_hasSound = false;

    // Sprite/visuel du PNJ
    sf::RectangleShape          m_visual;

    bool                        m_boxInitialized = false;
};