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
    m_darkness(windowSize, 250.f, 80.f),
    m_gameMode(GameMode::TPS)
{
    if (!m_font.openFromFile("assets/fonts/Pixellettersfull-BnJ5.ttf")) {}

    m_hintText = new sf::Text(m_font, "", 18);
    m_hintText->setFillColor(sf::Color::White);
    m_hintText->setPosition(sf::Vector2f(20.f, 20.f));

    texture = new sf::Texture("assets/pictures/EVA_maps.png");
    bg = new sf::Sprite(*texture);
    bg->setScale(sf::Vector2f(1.5f, 1.f));
    m_sceneTexture.resize(windowSize);

    m_npcs.push_back(new NPC(
        1056.f, 668.f,
        { "Bonjour",
          "J'aimerai faire une partie",
          "Bien sur ! Vous connaissez les regles ?",
          "Heu, non",
          "Il  est interdit de courir, c'est la regle la plus importante",
          "OK, c'est compris",
          "Bien vous pouvez allez sur l'arene si vous voulez jouer" },
        { "",
          "",
          "assets/pictures/Barman.png",
          "" },
        { 0, 0, 1, 0, 1, 0, 1 },
        "assets/pictures/BoysFace1.png",
        m_font
    ));

    PNJ* pnj = new PNJ(632.f, 358.f, "character-spritesheet2.png");
    pnj->SetWaypoints({
        sf::Vector2f(632.f, 358.f),
        sf::Vector2f(866.f, 358.f),
        sf::Vector2f(866.f, 250.f)
        });
    m_pnjs.push_back(pnj);

    PNJ* pnj1 = new PNJ(425.f, 194.f, "character-spritesheet3.png");
    pnj1->SetWaypoints({
        sf::Vector2f(425.f, 194.f),
        sf::Vector2f(296.f, 298.f),
        sf::Vector2f(526.f, 376.f)
        });
    m_pnjs.push_back(pnj1);

    PNJ* pnj2 = new PNJ(1055.f, 321.f, "character-spritesheet4.png");
    pnj2->SetWaypoints({
        sf::Vector2f(1055.f, 321.f),
        sf::Vector2f(820.f, 260.f),
        sf::Vector2f(936.f, 387.f)
        });
    m_pnjs.push_back(pnj2);

    PNJ* pnj3 = new PNJ(571.f, 259.f, "character-spritesheet5.png");
    pnj3->SetWaypoints({
        sf::Vector2f(571.f, 259.f),
        sf::Vector2f(396.f, 402.f),
        sf::Vector2f(1016.f, 414.f)
        });
    m_pnjs.push_back(pnj3);
}

Game::~Game() {
    delete texture;    texture = nullptr;
    delete bg;         bg = nullptr;
    delete m_hintText; m_hintText = nullptr;
    for (auto* npc : m_npcs) delete npc;
    m_npcs.clear();
    for (auto* pnj : m_pnjs) delete pnj;
    m_pnjs.clear();
}

void Game::SwitchToSurvival(std::vector<Level*>& levels) {
    m_gameMode = GameMode::SURVIVAL;
    currentLvl = 1;
    // Charge le bg une seule fois
    delete bg; delete texture;
    texture = new sf::Texture("assets/pictures/Zombie_EVA.png");
    bg = new sf::Sprite(*texture);
    bg->setScale(sf::Vector2f(1.5f, 1.5f));
    bg->setPosition(sf::Vector2f(-350.f, -250.f));
    levels[1]->SetOffset(-350.f, -250.f);
}

void Game::SwitchToBattle() {
    m_gameMode = GameMode::BATTLE;
    currentLvl = 2;
    delete bg; delete texture;
    texture = new sf::Texture("assets/pictures/Battle_EVA.png");
    bg = new sf::Sprite(*texture);
    bg->setScale(sf::Vector2f(1.5f, 1.f));
    bg->setPosition(sf::Vector2f(0.f, 0.f));
}

void Game::SwitchToTPS(Camera* camera) {
    m_gameMode = GameMode::TPS;
    currentLvl = 0;
    camera->SetZoom(1.0f); // zoom normal
    delete bg; delete texture;
    texture = new sf::Texture("assets/pictures/EVA_maps.png");
    bg = new sf::Sprite(*texture);
    bg->setScale(sf::Vector2f(1.5f, 1.f));
}

void Game::Render(std::vector<Level*>& levels,
    sf::RenderWindow& window, Parallax* parallax, Camera* camera, CameraMenu* cameramenu,
    StartMenu* startmenu, EndMenu* endmenu, Button* start, Button* exit,
    Button* letscontinue, Button* zombie, Button* battle, Entity* player, ChooseGame* choose, std::vector<Shoot*>& shoot)
{
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
        m_sceneTexture.setView(camera->GetView());
        m_sceneTexture.draw(*bg);

        levels[currentLvl]->Render(m_sceneTexture);
        player->Render(&m_sceneTexture);
        for (int i = 0; i < shoot.size(); ++i) {
            shoot[i]->Render(window);
        }

        if (m_gameMode == GameMode::TPS) {
            for (auto* npc : m_npcs) npc->Render(&m_sceneTexture);
            for (auto* pnj : m_pnjs) pnj->Render(&m_sceneTexture);
        }

        m_sceneTexture.display();
        window.setView(window.getDefaultView());
        m_darkness.Render(window, m_sceneTexture, camera->GetView());

        sf::Sprite sceneSprite(m_sceneTexture.getTexture());
        window.draw(sceneSprite);

        if (m_gameMode == GameMode::TPS) {
            for (auto* npc : m_npcs)
                npc->RenderBubble(&m_sceneTexture, window, camera->GetView());
            window.setView(window.getDefaultView());
            window.draw(*m_hintText);
            m_hintText->setString("");
        }
        break;
    }
    case CHOOSE_YESNO: {
        m_sceneTexture.clear(sf::Color::Black);
        m_sceneTexture.setView(camera->GetView());
        m_sceneTexture.draw(*bg);
        levels[currentLvl]->Render(m_sceneTexture);
        player->Render(&m_sceneTexture);
        for (auto* npc : m_npcs) npc->Render(&m_sceneTexture);
        for (auto* pnj : m_pnjs) pnj->Render(&m_sceneTexture);
        m_sceneTexture.display();

        window.setView(window.getDefaultView());
        sf::Sprite sceneSprite(m_sceneTexture.getTexture());
        window.draw(sceneSprite);

        if (m_activeNPC) m_activeNPC->RenderBubble(&m_sceneTexture, window, camera->GetView());

        m_hintText->setString("Veux-tu jouer ? O = Oui / N = Non");
        window.draw(*m_hintText);
        break;
    }
    case CHOOSE_MODE: {
        m_sceneTexture.clear(sf::Color::Black);
        m_sceneTexture.setView(camera->GetView());
        m_sceneTexture.draw(*bg);
        levels[currentLvl]->Render(m_sceneTexture);
        player->Render(&m_sceneTexture);
        for (auto* npc : m_npcs) npc->Render(&m_sceneTexture);
        for (auto* pnj : m_pnjs) pnj->Render(&m_sceneTexture);
        m_sceneTexture.display();

        window.setView(window.getDefaultView());
        sf::Sprite sceneSprite(m_sceneTexture.getTexture());
        window.draw(sceneSprite);

        window.setView(cameramenu->GetMenuView());
        choose->Render(window);
        zombie->Render(window);
        battle->Render(window);
        break;
    }
    case SURVIVALS: {
        m_sceneTexture.clear(sf::Color::Black);
        m_sceneTexture.setView(camera->GetView());
        m_sceneTexture.draw(*bg);
        levels[currentLvl]->Render(m_sceneTexture);
        player->Render(&m_sceneTexture);

        for (auto* e : m_ennemis)
            e->Render(&m_sceneTexture);

        for (auto* s : shoot)
            s->Render(m_sceneTexture);

        m_sceneTexture.display();
        window.setView(window.getDefaultView());
        m_darkness.Render(window, m_sceneTexture, camera->GetView());

        sf::Sprite sceneSprite(m_sceneTexture.getTexture());
        window.draw(sceneSprite);
        break;
    }
    case BATTLES: {
        m_sceneTexture.clear(sf::Color::Black);
        m_sceneTexture.setView(camera->GetView());
        m_sceneTexture.draw(*bg);
        levels[currentLvl]->Render(m_sceneTexture);
        player->Render(&m_sceneTexture);
        for (auto* s : shoot)       
            s->Render(window);

        m_sceneTexture.display();
        window.setView(window.getDefaultView());
        m_darkness.Render(window, m_sceneTexture, camera->GetView());

        sf::Sprite sceneSprite(m_sceneTexture.getTexture());
        window.draw(sceneSprite);
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

void Game::Update(bool& isRunning, bool& isEnd, float dt, float now,
    std::vector<sf::Event> events, std::vector<Level*>& levels,
    sf::RenderWindow& window, Parallax* parallax, Camera* camera,
    CameraMenu* cameramenu, SoundManager& sound, Button* start,
    Button* exit, Button* letscontinue, Button* zombie, Button* battle,
    Entity* player, ChooseGame* choose, std::vector<Shoot*>& shoot)
{
    switch (state) {
    case RUNNING: {
        sf::Vector2f playerPos = player->rect.getPosition();
        sf::Vector2f playerSize = player->rect.getSize();

        if (m_gameMode == GameMode::TPS) {
            bool onHZone = false;
            levels[currentLvl]->Update(currentLvl, newLvl, dt, now,
                playerPos.x, playerPos.y, playerSize.x, onHZone);
            camera->SetCeilingMode(onHZone);

            if (newLvl - currentLvl > 0 && newLvl < (int)levels.size() ||
                newLvl - currentLvl < 0 && newLvl >= 0)
                currentLvl = newLvl;

            player->Update(dt, window.getSize(), levels[currentLvl]->GetBlocks(), shoot, false);
            playerPos = player->rect.getPosition();

            bool ePressed = m_input.checkInteractionPressed();
            sf::Vector2f playerCenter = playerPos + sf::Vector2f(player->width / 2.f, player->height / 2.f);

            bool anyNPCNear = false;
            bool anyDialogueOpen = false;

            for (auto* npc : m_npcs) {
                npc->Update(dt, window.getSize(), levels[currentLvl]->GetBlocks(), shoot, false);

                if (npc->IsPlayerNear(playerCenter, 100.f)) {
                    anyNPCNear = true;
                    if (npc->IsDialogueOpen()) anyDialogueOpen = true;

                    if (ePressed) {
                        if (npc->IsDialogueOpen()) {
                            if (npc->IsLastDialogue()) {
                                m_activeNPC = npc;
                                state = CHOOSE_YESNO;
                            }
                            else {
                                npc->NextDialogue();
                            }
                        }
                        else {
                            npc->OpenDialogue();
                        }
                    }
                }
                else if (npc->IsDialogueOpen()) {
                    npc->CloseDialogue();
                }
            }

            for (auto* pnj : m_pnjs)
                pnj->Update(dt, window.getSize(), levels[currentLvl]->GetBlocks(), shoot, false);

            if (anyNPCNear && !anyDialogueOpen) {
                m_hintText->setString("Press E to interact");
                sf::Vector2f worldPos = playerCenter + sf::Vector2f(-30.f, -30.f);
                sf::Vector2i screenPos = window.mapCoordsToPixel(worldPos, camera->GetView());
                m_hintText->setPosition(sf::Vector2f(screenPos));
            }

            for (auto& b : levels[currentLvl]->GetBlocks()) {
                if (b->GetBlockType() == "EndBlock") {
                    sf::FloatRect playerBounds = player->rect.getGlobalBounds();
                    sf::FloatRect blockBounds = b->rect.getGlobalBounds();
                    if (playerBounds.findIntersection(blockBounds)) {
                        SwitchToSurvival(levels);
                    }
                }
            }

            sf::Vector2f camCenter = camera->GetView().getCenter();
            parallax->Update(camCenter.x, camCenter.y);
            camera->Update(playerPos.x + playerSize.x / 2.f, playerPos.y + playerSize.y / 2.f, dt);
        }
        else if (m_gameMode == GameMode::SURVIVAL || m_gameMode == GameMode::BATTLE) {
            bool onHZone = false;
            levels[currentLvl]->Update(currentLvl, newLvl, dt, now,
                playerPos.x, playerPos.y, playerSize.x, onHZone);
            camera->SetCeilingMode(onHZone);

            if (newLvl - currentLvl > 0 && newLvl < (int)levels.size() ||
                newLvl - currentLvl < 0 && newLvl >= 0)
                currentLvl = newLvl;

            player->Update(dt, window.getSize(), levels[currentLvl]->GetBlocks(), shoot, true);
            m_darkness.setPlayerPos(player->rect.getPosition()
                + sf::Vector2f(player->width / 2.f, player->height / 2.f));

            playerPos = player->rect.getPosition();

            for (Shoot* s : shoot) {
                if (s != nullptr) {
                    s->Update(dt, now);
                }
            }

            shoot.erase(
                std::remove_if(shoot.begin(), shoot.end(), [](Shoot* s) {
                    return !s->alive;
                    }),
                shoot.end()
            );
            sf::Vector2f camCenter = camera->GetView().getCenter();
            parallax->Update(camCenter.x, camCenter.y);
            camera->Update(playerPos.x + playerSize.x / 2.f, playerPos.y + playerSize.y / 2.f, dt);
        }

        gameTime = now;
        if (isEnd) state = END;
        break;
    }

    case CHOOSE_YESNO: {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O)) {
            if (m_activeNPC) m_activeNPC->CloseDialogue();
            state = CHOOSE_MODE;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N)) {
            if (m_activeNPC) m_activeNPC->CloseDialogue();
            m_activeNPC = nullptr;
            state = RUNNING;
        }
        break;
    }
    case CHOOSE_MODE: {
        sf::View menuview = cameramenu->GetMenuView();
        zombie->UpdateHover(window, menuview);
        battle->UpdateHover(window, menuview);

        sf::Vector2f playerPos = player->rect.getPosition();
        sf::Vector2f playerSize = player->rect.getSize();

        bool onHZone = false;
        levels[currentLvl]->Update(currentLvl, newLvl, dt, now,
            playerPos.x, playerPos.y, playerSize.x, onHZone);
        camera->SetCeilingMode(onHZone);

        if (newLvl - currentLvl > 0 && newLvl < (int)levels.size() ||
            newLvl - currentLvl < 0 && newLvl >= 0)
            currentLvl = newLvl;

        player->Update(dt, window.getSize(), levels[currentLvl]->GetBlocks(), shoot, false);
        playerPos = player->rect.getPosition();

        sf::Vector2f camCenter = camera->GetView().getCenter();
        parallax->Update(camCenter.x, camCenter.y);
        camera->Update(playerPos.x + playerSize.x / 2.f, playerPos.y + playerSize.y / 2.f, dt);

        for (auto& ev : events) {
            if (auto* mb = ev.getIf<sf::Event::MouseButtonPressed>()) {
                if (mb->button == sf::Mouse::Button::Left) {
                    if (zombie->IsHovered()) {
                        SwitchToSurvival(levels);
                        player->rect.setPosition(sf::Vector2f(802.f, 926.f)); // reset spawn zombie
                        camera->SetZoom(1.25f);
                        camera->Update(802.f, 926.f, 0.f); // force la caméra sur le bon spawn
                        m_activeNPC = nullptr;
                        state = SURVIVALS;
                    }
                    else if (battle->IsHovered()) {
                        SwitchToBattle();   
                        player->rect.setPosition(sf::Vector2f(1057.f, 768.f));
                        camera->SetZoom(1.5f);
                        camera->Update(1057.f, 768.f, 0.f);
                        m_activeNPC = nullptr;
                        state = BATTLES;
                    }
                }
            }
        }
        break;
    }
    case SURVIVALS: {
        sf::Vector2f playerPos = player->rect.getPosition();
        sf::Vector2f playerSize = player->rect.getSize();

        bool onHZone = false;
        levels[currentLvl]->Update(currentLvl, newLvl, dt, now,
            playerPos.x, playerPos.y, playerSize.x, onHZone);
        camera->SetCeilingMode(onHZone);

        player->Update(dt, window.getSize(), levels[currentLvl]->GetBlocks(), shoot, true);
        m_darkness.setPlayerPos(player->rect.getPosition()
            + sf::Vector2f(player->rect.getSize().x / 2.f, player->rect.getSize().y / 2.f));

        playerPos = player->rect.getPosition();

        // Position de spawn des ennemis
        float spawnX = 1689.f;
        float spawnY = 540.f;
        float spawnXx = -65.f;
        float spawnYy = 375.f;

        m_spawner.Update(dt, m_ennemis, spawnX, spawnY);
        m_spawner2.Update(dt, m_ennemis, spawnXx, spawnYy);

        // Update + collisions + cible = joueur pour le pattern Kamikaze
        sf::Vector2f playerCenter = playerPos + sf::Vector2f(playerSize.x / 2.f, playerSize.y / 2.f);
        for (Ennemi* e : m_ennemis) {
            e->Update(dt, playerCenter);
            e->ResolveCollisions(levels[currentLvl]->GetBlocks());
        }

        for (Shoot* s : shoot) {
            if (s != nullptr) {
                s->Update(dt, now);
            }
        }

        shoot.erase(
            std::remove_if(shoot.begin(), shoot.end(), [](Shoot* s) {
                return !s->alive;
                }),
            shoot.end()
        );

        sf::Vector2f camCenter = camera->GetView().getCenter();
        parallax->Update(camCenter.x, camCenter.y);
        camera->Update(playerPos.x + playerSize.x / 2.f, playerPos.y + playerSize.y / 2.f, dt);

        gameTime = now;
        if (isEnd) state = END;
        break;
    }
    case BATTLES: {
        sf::Vector2f playerPos = player->rect.getPosition();
        sf::Vector2f playerSize = player->rect.getSize();

        bool onHZone = false;
        levels[currentLvl]->Update(currentLvl, newLvl, dt, now,
            playerPos.x, playerPos.y, playerSize.x, onHZone);
        camera->SetCeilingMode(onHZone);

        player->Update(dt, window.getSize(), levels[currentLvl]->GetBlocks(), shoot, true);
        m_darkness.setPlayerPos(player->rect.getPosition()
            + sf::Vector2f(player->rect.getSize().x / 2.f, player->rect.getSize().y / 2.f));

        playerPos = player->rect.getPosition();

        for (Shoot* s : shoot) {
            if (s != nullptr) {
                s->Update(dt, now);
            }
        }

        shoot.erase(
            std::remove_if(shoot.begin(), shoot.end(), [](Shoot* s) {
                return !s->alive;
                }),
            shoot.end()
        );

        sf::Vector2f camCenter = camera->GetView().getCenter();
        parallax->Update(camCenter.x, camCenter.y);
        camera->Update(playerPos.x + playerSize.x / 2.f, playerPos.y + playerSize.y / 2.f, dt);
        gameTime = now;
        if (isEnd) state = END;
        break;
    }
    case END: {
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
    case MENU: {
        sf::View menuview = cameramenu->GetMenuView();
        start->UpdateHover(window, menuview);
        exit->UpdateHover(window, menuview);

        if (isRunning) {
            state = RUNNING;
        }
        break;
    }
    }
}