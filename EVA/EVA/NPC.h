#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include "Entity.h"

class NPC : public Entity {
public:
    NPC(float x, float y, std::vector<std::string> dialogues, std::vector<std::string> faces, std::vector<int> speakers, const std::string& playerFacePath, sf::Font& font);
    void RenderBubble(sf::RenderTarget* target, sf::RenderWindow& window, sf::View cameraView);
    void Update(float dt, sf::Vector2u windowSize, std::vector<Block*>& blocks, std::vector<Shoot*>& shoot, bool canShoot = false) override;
    void Render(sf::RenderTarget* target) override;

    bool IsPlayerNear(sf::Vector2f playerPos, float range = 100.f);
    bool NextDialogue();
    void OpenDialogue();    
    void CloseDialogue();
    bool IsDialogueOpen() const;
    bool IsLastDialogue() const;
    void SetPlayerFace(const std::string& facePath);

private:
    void SetupDialogBox(unsigned int winW, unsigned int winH);
    void StartLine();
    void UpdateTyping(float dt);

    std::vector<std::string>    m_dialogues;
    std::vector<std::string>    m_faces;
    std::vector<int>            m_speakers;  // 0 = joueur, 1 = NPC, par ligne
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

    // Portrait NPC (droite)
    sf::Texture                 m_faceTexture;
    sf::RectangleShape          m_faceRect;

    // Portrait joueur (gauche)
    sf::Texture                 m_playerFaceTexture;
    sf::RectangleShape          m_playerFaceRect;
    bool                        m_hasPlayerFace = false;

    std::string                 m_fullText;
    int                         m_visibleChars = 0;
    float                       m_charTimer = 0.f;
    float                       m_charDelay = 0.05f;
    bool                        m_isTyping = false;

    sf::Music                   m_dial;
    bool                        m_hasSound = false;

    sf::RectangleShape          m_visual;
    bool                        m_boxInitialized = false;
};

class ChoiceDialog {
public:
    enum class Choice { None, PlayALittle, Work };

    bool visible = false;
    bool prevPressed = false;
    Choice chosen = Choice::None;

    sf::RectangleShape panel;
    sf::Texture texturePanel;

    sf::RectangleShape buttonPlay;
    sf::RectangleShape buttonWork;
    sf::Texture textureButtonPlay;
    sf::Texture textureButtonWork;

    sf::Font font;
    sf::Text* lblPlay = nullptr;
    sf::Text* lblWork = nullptr;
    sf::Text* prompt = nullptr;


    sf::FloatRect playBounds;
    sf::FloatRect workBounds;

public:
    ChoiceDialog();

    void init(unsigned int winW, unsigned int winH);

    bool isVisible() const;
    void show();
    void hide();
    Choice getChoice() const;

    Choice update(const sf::RenderWindow& window);

    void render(sf::RenderWindow& window);
};