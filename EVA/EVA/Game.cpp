#include "Game.h"
#include <fstream>
#include <string>

#include "StartMenu.h"
#include "EndMenu.h"
#include "SoundManager.h"
#include "Button.h"
#include "CameraMenu.h"
#include "Camera.h"
#include "Parallax.h"
#include "Block.h"
#include "Level.h"
#include "Entity.h"
#include <random>
#include <algorithm>
#include <iostream>


Game::Game(sf::Vector2u windowSize)
    : newLvl(0), currentLvl(0), gameTime(0), state(MENU),
    //m_darkness(windowSize, 250.f, 80.f),
    m_gameMode(GameMode::TPS),  // démarre en TPS
    m_player2d(nullptr)
{
    if (!m_font.openFromFile("assets/fonts/arial.ttf")) {}

    m_hintText = new sf::Text(m_font, "", 18);
    m_hintText->setFillColor(sf::Color::White);
    m_hintText->setPosition(sf::Vector2f(20.f, 20.f)); // position à l'écran

    texture = new sf::Texture("assets/pictures/EVA_maps.png");
    bg = new sf::Sprite(*texture);
    bg->setScale(sf::Vector2f(1.5f, 1.f));
    m_sceneTexture.resize(windowSize);

    // Caméra platformer
    m_platformerCamera = sf::View(sf::FloatRect(
        { 0.f, 0.f },
        { static_cast<float>(windowSize.x), static_cast<float>(windowSize.y) }
    ));

    // NPC
    m_npcs.push_back(new NPC(
        1056.f, 768.f,
        { "Qui êtes vous ?", "Un humain...", "J'ai une requête pour toi.", "Si tu veux bien l'accepté.", "Peut tu trouver mon amie ketchup", "Le dernier endroit où je l'ai vu,", "C'était devant une porte qui menée sur un mur", "Mais aucun passage derrière", "S'il te palît, retrouve le"},
        m_font
    ));

    //PNJ
    PNJ* pnj = new PNJ(632.f, 358.f);
    pnj->SetWaypoints({
    sf::Vector2f(632.f, 358.f),
    sf::Vector2f(866.f, 358.f),
    sf::Vector2f(866.f, 250.f)
        });
    m_pnjs.push_back(pnj);

    // Tiles platformer (inactives jusqu'au switch)
    m_tiles.emplace_back(0.f, 550.f, 100.f, 32.f);
    m_tiles.emplace_back(200.f, 430.f, 128.f, 32.f);
    m_tiles.emplace_back(500.f, 350.f, 128.f, 32.f);
    m_tiles.emplace_back(800.f, 300.f, 50.f, 50.f);
    m_tiles.emplace_back(1000.f, 200.f, 50.f, 50.f);
    m_tiles.emplace_back(800.f, 50.f, 50.f, 50.f);
    m_tiles.emplace_back(1000.f, -100.f, 50.f, 50.f);
    m_tiles.emplace_back(1420.f, 400.f, 128.f, 32.f);
    m_tiles.emplace_back(1800.f, 400.f, 50.f, 50.f);
    m_tiles.emplace_back(1900.f, 230.f, 50.f, 50.f);
    m_tiles.emplace_back(2000.f, 400.f, 50.f, 50.f);
    m_tiles.emplace_back(2100.f, 230.f, 50.f, 50.f);
    m_tiles.emplace_back(2200.f, 400.f, 50.f, 50.f);
    m_tiles.emplace_back(2300.f, 230.f, 50.f, 50.f);
    m_tiles.emplace_back(2400.f, 400.f, 50.f, 50.f);
    m_tiles.emplace_back(2700.f, 400.f, 50.f, 50.f);
    m_tiles.emplace_back(3000.f, 400.f, 128.f, 32.f);
}

Game::~Game() {
    delete m_player2d; m_player2d = nullptr;
    delete texture;    texture = nullptr;
    delete bg;         bg = nullptr;
    delete m_hintText; m_hintText = nullptr;
    for (auto* npc : m_npcs) delete npc;
    m_npcs.clear();
    for (auto* pnj : m_pnjs) delete pnj;
    m_pnjs.clear();
}

void Game::SwitchToPlatformer() {
    m_gameMode = GameMode::PLATFORMER;
    // Crée le player2d au début du niveau platformer
    if (m_player2d) delete m_player2d;
    m_player2d = new Player2d(50.f, 550.f);
}

void Game::SwitchToTPS() {
    m_gameMode = GameMode::TPS;
    delete m_player2d;
    m_player2d = nullptr;
}

void Game::Render(std::vector<Level*>& levels,
    sf::RenderWindow& window, Parallax* parallax, Camera* camera,CameraMenu* cameramenu, StartMenu* startmenu, EndMenu* endmenu, Button* start, Button* exit, Button* letscontinue, Entity* player) {
    switch (state) {
    case MENU: {
        window.setView(cameramenu->GetMenuView());
        startmenu->Render(window);
        start->Render(window);
        exit->Render(window);
        break;
    }
    case RUNNING: {
        m_sceneTexture.clear(sf::Color::Black);

        if (m_gameMode == GameMode::TPS) {
            // MODE TPS
            m_sceneTexture.setView(camera->GetView());
            m_sceneTexture.draw(*bg);
            levels[currentLvl]->Render(m_sceneTexture);
            player->Render(&m_sceneTexture);
            for (auto* npc : m_npcs)
                npc->Render(&m_sceneTexture);
            for (auto* pnj : m_pnjs)
                pnj->Render(&m_sceneTexture);
        }
        else {
            // MODE PLATFORMER
            m_sceneTexture.setView(m_platformerCamera);
            for (auto& tile : m_tiles)
                tile.Render(&m_sceneTexture);
            if (m_player2d)
                m_player2d->Render(&m_sceneTexture);
        }

        m_sceneTexture.display();
        window.setView(window.getDefaultView());
        //m_darkness.Render(window, m_sceneTexture, m_gameMode == GameMode::TPS ? camera->GetView() : m_platformerCamera);

        // Rendu direct de la scène sans ombre
        sf::Sprite sceneSprite(m_sceneTexture.getTexture());
        window.draw(sceneSprite);

        if (m_gameMode == GameMode::TPS) {
            for (auto* npc : m_npcs)
                npc->RenderBubble(&m_sceneTexture, window, camera->GetView());

            // Hint "Appuie sur E"
            window.setView(window.getDefaultView());
            window.draw(*m_hintText);
m_hintText->setString("");

            // Reset le hint chaque frame (Update le remet si un NPC est proche)
            m_hintText->setString("");
        }
        break;
    }
    case END:
        window.setView(cameramenu->GetMenuView());
        endmenu->Render(window);
        letscontinue->Render(window);
        exit->Render(window);
        break;
    }
}

void Game::Update(bool& isRunning, bool& isEnd,float dt, float now,
    std::vector<sf::Event> events, std::vector<Level*>& levels,
    sf::RenderWindow& window, Parallax* parallax, Camera* camera, CameraMenu* cameramenu, SoundManager& sound, Button* start, Button* exit, Button* letscontinue, Entity* player) {
    switch (state) {
    case RUNNING: {
        if (m_gameMode == GameMode::TPS) {
            // === MODE TPS ===
            levels[currentLvl]->Update(currentLvl, newLvl, dt, now);
            if (newLvl - currentLvl > 0 && newLvl < (int)levels.size() ||
                newLvl - currentLvl < 0 && newLvl >= 0)
                currentLvl = newLvl;

            player->Update(dt, window.getSize(), levels[currentLvl]->GetBlocks());
            //m_darkness.setPlayerPos(player->rect.getPosition() + sf::Vector2f(player->width / 2.f, player->height / 2.f));

            // NPC interaction
            bool ePressed = m_input.checkInteractionPressed();
            sf::Vector2f playerCenter = player->rect.getPosition()
                + sf::Vector2f(player->width / 2.f, player->height / 2.f);

            bool anyNPCNear = false;
            bool anyDialogueOpen = false;

            for (auto* npc : m_npcs) {
                npc->Update(dt, window.getSize(), levels[currentLvl]->GetBlocks());

                if (npc->IsPlayerNear(playerCenter, 100.f)) {
                    anyNPCNear = true;
                    if (npc->IsDialogueOpen()) anyDialogueOpen = true;
                    if (ePressed) {
                        if (npc->IsDialogueOpen()) npc->NextDialogue();
                        else npc->OpenDialogue();
                    }
                }
                else if (npc->IsDialogueOpen()) {
                    npc->CloseDialogue();
                }
            }

            for (auto* pnj : m_pnjs)
                pnj->Update(dt, window.getSize(), levels[currentLvl]->GetBlocks());

            if (anyNPCNear && !anyDialogueOpen) {
                m_hintText->setString("Press E to interact");
                sf::Vector2f worldPos = playerCenter + sf::Vector2f(-30.f, -30.f);
                sf::Vector2i screenPos = window.mapCoordsToPixel(worldPos, camera->GetView());
                m_hintText->setPosition(sf::Vector2f(screenPos));
            }

            // Détection porte ? switch platformer
            // Appelle SwitchToPlatformer() quand le joueur touche la porte
            for (auto& b : levels[currentLvl]->GetBlocks()) {
                if (b->GetBlockType() == "EndBlock") {
                    sf::FloatRect playerBounds = player->rect.getGlobalBounds();
                    sf::FloatRect blockBounds = b->rect.getGlobalBounds();
                    if (playerBounds.findIntersection(blockBounds)) {
                        SwitchToPlatformer();
                    }
                }
            }

            sf::Vector2f camCenter = camera->GetView().getCenter();
            parallax->Update(camCenter.x, camCenter.y);
            sf::Vector2f playerPos = player->rect.getPosition();
            sf::Vector2f playerSize = player->rect.getSize();
            camera->Update(playerPos.x + playerSize.x / 2.f,
                playerPos.y + playerSize.y / 2.f, dt);

        }
        else {
            // === MODE PLATFORMER ===
            for (auto& event : events)
                m_player2d->handleEvent(event);

            m_player2d->Update(dt, window.getSize(), m_tiles);

            // Caméra platformer suit Player2d
            sf::Vector2f pos = m_player2d->getPosition();
            sf::Vector2f camCenter = m_platformerCamera.getCenter();
            camCenter.x += (pos.x - camCenter.x) * 0.5f;
            camCenter.y += ((pos.y - 30.f) - camCenter.y) * 0.5f;
            m_platformerCamera.setCenter(camCenter);

            //m_darkness.setPlayerPos(pos);
        }

        gameTime = now;
        if (isEnd) state = END;
        break;
    }

    case END: {
        /*if (!sound.IsMusicPlaying()) {
            sound.PlayMusic("assets/sounds/end_song.ogg", false); //pas de loop
        }*/
        if (!isEnd) {
            sound.StopMusic();
            state = RUNNING;
        }
        if (!isRunning && !isEnd) {
            sound.StopMusic();
            state = MENU;
        }
        break;
    }
    case MENU:
    {
        // Lancer musique menu si pas deja en cours
       /*if (!sound.IsMusicPlaying()) {
            sound.PlayMusic("assets/sounds/Menu.ogg", true);
        }*/
        sf::View menuview = cameramenu->GetMenuView();
        start->UpdateHover(window, menuview);
        exit->UpdateHover(window, menuview);


        if (isRunning) {
            //sound.StopMusic();
            state = RUNNING;
        }
        break;
    }
    }
}
