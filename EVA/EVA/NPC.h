#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Entity.h"

class NPC : public Entity {
public:
    NPC(float x, float y, std::vector<std::string> dialogues, sf::Font& font);

    void RenderBubble(sf::RenderTarget* target, sf::RenderWindow& window, sf::View cameraView);

    void Update(float dt, sf::Vector2u windowSize, std::vector<Block*>& blocks) override;
    void Render(sf::RenderTarget* target) override;

    // Retourne true si le joueur est assez proche
    bool IsPlayerNear(sf::Vector2f playerPos, float range = 100.f);

    // Avance au dialogue suivant, retourne false si c'est fini
    bool NextDialogue();

    // Ouvre/ferme la bulle
    void OpenDialogue();
    void CloseDialogue();
    bool IsDialogueOpen() const;

private:
    void BuildBubble();

    std::vector<std::string>    m_dialogues;
    int                         m_currentLine;
    bool                        m_dialogueOpen;

    sf::Texture m_npcTexture;
    sf::Sprite* m_npcSprite = nullptr;

    sf::Font& m_font;

    // Bulle
    sf::RectangleShape          m_bubble;
    sf::Text                    m_text;
    sf::RectangleShape          m_tail; // petit triangle sous la bulle

    // Sprite/visuel du PNJ
    sf::RectangleShape          m_visual;
};