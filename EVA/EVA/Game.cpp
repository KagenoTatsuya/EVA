#include "Game.h"
#include <fstream>
#include <string>
#include "PvE.h"
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


int vie = 3;
int score = 0;

Game::Game(sf::Vector2u windowSize)
    : newLvl(0), currentLvl(0), gameTime(0), state(MENU),
    m_darkness(windowSize, 250.f, 80.f),
    m_gameMode(GameMode::TPS), hud(m_font)
{
    if (!m_font.openFromFile("assets/fonts/Pixellettersfull-BnJ5.ttf")) {}

    m_hintText = new sf::Text(m_font, "", 18);
    m_hintText->setFillColor(sf::Color::White);
    m_hintText->setPosition(sf::Vector2f(20.f, 20.f));

    texture = new sf::Texture("assets/pictures/EVA_maps.png");
    bg = new sf::Sprite(*texture);
    bg->setScale(sf::Vector2f(1.5f, 1.f));
    m_sceneTexture.resize(windowSize);
    m_pause = new Pause(m_font);

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

    PNJ* pnj4 = new PNJ(680.f, 558.f, "character-spritesheet13.png");
    pnj4->SetWaypoints({
        sf::Vector2f(680.f, 558.f),
        sf::Vector2f(1077.f, 1113.f),
        sf::Vector2f(975.f, 54.f)
        });
    m_pnjs.push_back(pnj4);

    PNJ* pnj5 = new PNJ(197.f, 834.f, "character-spritesheet14.png");
    pnj5->SetWaypoints({
        sf::Vector2f(197.f, 834.f),
        sf::Vector2f(570.f, 698.f),
        sf::Vector2f(555.f, 699.f)
        });
    m_pnjs.push_back(pnj5);
}

Game::~Game() {
    delete texture;    texture = nullptr;
    delete bg;         bg = nullptr;
    delete m_hintText; m_hintText = nullptr;
    delete m_pause; m_pause = nullptr;
    for (auto* npc : m_npcs) delete npc;
    m_npcs.clear();
    for (auto* pnj : m_pnjs) delete pnj;
    m_pnjs.clear();
    for (auto* p : m_soldatProjectiles) delete p;
    m_soldatProjectiles.clear();
}

void Game::SwitchToSurvival(std::vector<Level*>& levels) {
    m_gameMode = GameMode::SURVIVAL;
    currentLvl = 1;
    m_darkness.setRadius(180.f);

    m_score = 0;
    m_vies = 3;
    m_invincibleTimer = 0.f;

    // Nettoyage des ennemis d'une partie Survival précédente
    for (Ennemi* e : m_ennemis) delete e;
    m_ennemis.clear();

    delete bg; delete texture;
    texture = new sf::Texture("assets/pictures/Zombie_EVA.png");
    bg = new sf::Sprite(*texture);
    bg->setScale(sf::Vector2f(1.5f, 1.5f));
    bg->setPosition(sf::Vector2f(-350.f, -250.f));
    levels[1]->SetOffset(-350.f, -250.f);
}

void Game::SwitchToBattle(std::vector<Level*>& levels) {
    m_gameMode = GameMode::BATTLE;
    currentLvl = 2;

    m_score = 0;
    m_vies = 3;
    m_invincibleTimer = 0.f;
    m_battleTimer = BATTLE_DURATION;
    m_battleResultText.clear();

    for (Soldat* s : m_soldat) delete s;
    m_soldat.clear();

    delete bg; delete texture;
    texture = new sf::Texture("assets/pictures/Battle_arene.png");
    bg = new sf::Sprite(*texture);
    bg->setScale(sf::Vector2f(1.5f, 1.5f));
    bg->setPosition(sf::Vector2f(-350.f, -250.f));
    levels[2]->SetOffset(-355.f, -270.f);

    m_zoneManager.ExtractZonesFromFile("Level3.txt", { {'K', "Zone B"}, {'J', "Zone C"} }, 24.f, { -355.f, -270.f });
    m_zoneManager.ExtractWaypointsFromFile("Level3.txt", 24.f, { -355.f, -270.f });
    m_zoneManager.ResetZones();

    // Cache les murs une seule fois : évite de rescanner TOUS les blocks
    // (y compris les tuiles vides) 34 fois par soldat à chaque frame
    m_battleWalls.clear();
    for (Block* b : levels[2]->GetBlocks()) {
        if (b->GetBlockType() == "MBlock") {
            m_battleWalls.push_back(b);
        }
    }
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
    Button* letscontinue, Button* zombie, Button* arene, Entity* player, ChooseGame* choose, std::vector<Shoot*>& shoot)
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
        arene->Render(window);
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

        /*sf::Sprite sceneSprite(m_sceneTexture.getTexture());
        window.draw(sceneSprite);*/
        hud.render(window);
        break;
    }
    case BATTLES: {
        m_sceneTexture.clear(sf::Color::Black);
        m_sceneTexture.setView(camera->GetView());
        m_sceneTexture.draw(*bg);
        levels[currentLvl]->Render(m_sceneTexture);
        player->Render(&m_sceneTexture);

        for (auto* t : m_soldat)
            t->Render(&m_sceneTexture);

        for (auto* p : m_soldatProjectiles)
            p->Render(&m_sceneTexture);

        for (auto* s : shoot)
            s->Render(m_sceneTexture);

        m_sceneTexture.display();
        window.setView(window.getDefaultView());

        sf::Sprite sceneSprite(m_sceneTexture.getTexture());
        window.draw(sceneSprite);

        // Affichage des jauges de capture pour Zone B et Zone C
        auto& zones = m_zoneManager.GetZones();
        float gaugeX = 40.f;
        for (const Zone& zone : zones) {
            hud.renderZoneGauge(window, zone, { gaugeX, 60.f }, { 40.f, 200.f });
            gaugeX += 80.f;
        }

        // Affichage du timer de partie (mode Battle)
        hud.renderTimer(window, m_battleTimer);

        break;
    }
    case SELECT_PERSO: {
        window.setView(cameramenu->GetMenuView());
        m_selectPerso.Render(window);
        break;
    }
    case PAUSE: {
        switch (m_stateBeforePause) {
        case RUNNING: {
            m_sceneTexture.clear(sf::Color::Black);
            m_sceneTexture.setView(camera->GetView());
            m_sceneTexture.draw(*bg);
            levels[currentLvl]->Render(m_sceneTexture);
            player->Render(&m_sceneTexture);
            if (m_gameMode == GameMode::TPS) {
                for (auto* npc : m_npcs) npc->Render(&m_sceneTexture);
                for (auto* pnj : m_pnjs) pnj->Render(&m_sceneTexture);
            }
            m_sceneTexture.display();
            window.setView(window.getDefaultView());
            m_darkness.Render(window, m_sceneTexture, camera->GetView());
            sf::Sprite sceneSprite(m_sceneTexture.getTexture());
            window.draw(sceneSprite);
            break;
        }
        case SURVIVALS: {
            m_sceneTexture.clear(sf::Color::Black);
            m_sceneTexture.setView(camera->GetView());
            m_sceneTexture.draw(*bg);
            levels[currentLvl]->Render(m_sceneTexture);
            player->Render(&m_sceneTexture);
            for (auto* e : m_ennemis) e->Render(&m_sceneTexture);
            m_sceneTexture.display();
            window.setView(window.getDefaultView());
            m_darkness.Render(window, m_sceneTexture, camera->GetView());
            hud.render(window);
            break;
        }
        case BATTLES: {
            m_sceneTexture.clear(sf::Color::Black);
            m_sceneTexture.setView(camera->GetView());
            m_sceneTexture.draw(*bg);
            levels[currentLvl]->Render(m_sceneTexture);
            player->Render(&m_sceneTexture);
            for (auto* t : m_soldat) t->Render(&m_sceneTexture);
            for (auto* p : m_soldatProjectiles) p->Render(&m_sceneTexture);
            // Affichage des jauges de capture pour Zone B et Zone C
            auto& zones = m_zoneManager.GetZones();
            float gaugeX = 40.f;
            for (const Zone& zone : zones) {
                hud.renderZoneGauge(window, zone, { gaugeX, 60.f }, { 40.f, 200.f });
                gaugeX += 80.f;
            }

            hud.renderTimer(window, m_battleTimer);
            m_sceneTexture.display();
            window.setView(window.getDefaultView());
            sf::Sprite sceneSprite(m_sceneTexture.getTexture());
            window.draw(sceneSprite);
            break;
        }
        default: break;
        }

        sf::View menuview = cameramenu->GetMenuView();
        m_pause->Render(window, menuview);
        break;
    }
    case END:
        window.setView(cameramenu->GetMenuView());
        endmenu->Render(window);

        if (m_gameMode == GameMode::BATTLE && !m_battleResultText.empty()) {
            sf::Text resultText(m_font, m_battleResultText, 48);
            resultText.setFillColor(sf::Color::Yellow);

            sf::FloatRect bounds = resultText.getLocalBounds();
            sf::Vector2f viewSize = cameramenu->GetMenuView().getSize();
            sf::Vector2f viewCenter = cameramenu->GetMenuView().getCenter();

            resultText.setOrigin(bounds.getCenter());
            resultText.setPosition(sf::Vector2f(viewCenter.x, viewCenter.y - viewSize.y / 2.f + 100.f));

            window.draw(resultText);
        }

        letscontinue->Render(window);
        exit->Render(window);
        break;
    }
}

void Game::Update(bool& isRunning, bool& isEnd, bool& isPause, float dt, float now,
    std::vector<sf::Event>& events, std::vector<Level*>& levels,
    sf::RenderWindow& window, Parallax* parallax, Camera* camera,
    CameraMenu* cameramenu, SoundManager& sound, Button* start,
    Button* exit, Button* letscontinue, Button* zombie, Button* arene,
    Entity* player, ChooseGame* choose, std::vector<Shoot*>& shoot)
{
    for (auto& ev : events) {
        if (auto* kp = ev.getIf<sf::Event::KeyPressed>()) {
            if (kp->code == sf::Keyboard::Key::Escape) {
                if (state == RUNNING || state == SURVIVALS || state == BATTLES) {
                    m_stateBeforePause = state;
                    state = PAUSE;
                }
                else if (state == PAUSE) {
                    state = m_stateBeforePause;
                }
            }
        }
    }

    isPause = (state == PAUSE);

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

            m_darkness.setPlayerPos(playerPos + sf::Vector2f(player->width / 2.f, player->height / 2.f));

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

            //for (auto& b : levels[currentLvl]->GetBlocks()) {
            //    if (b->GetBlockType() == "EndBlock") {
            //        sf::FloatRect playerBounds = player->rect.getGlobalBounds();
            //        sf::FloatRect blockBounds = b->rect.getGlobalBounds();      //==========================================END BLOCK FOIREUX===============================================
            //        if (playerBounds.findIntersection(blockBounds)) {
            //            SwitchToSurvival(levels);
            //        }
            //    }
            //}

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
            m_darkness.setPlayerPos(player->rect.getPosition() + sf::Vector2f(player->width / 2.f, player->height / 2.f));

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
        arene->UpdateHover(window, menuview);

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
                    else if (arene->IsHovered()) {
                        SwitchToBattle(levels);   
                        player->rect.setPosition(sf::Vector2f(1784.f, 475.f));
                        camera->SetZoom(1.44f);
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
        m_darkness.setPlayerPos(player->rect.getPosition() + sf::Vector2f(player->rect.getSize().x / 2.f, player->rect.getSize().y / 2.f));

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

        PvE::ResolveEnnemiCollisions(m_ennemis);

        for (Shoot* s : shoot) {
            if (s != nullptr) {
                s->Update(dt, now);
            }
        }

        for (auto& ev : events) {
            if (auto* mb = ev.getIf<sf::Event::MouseButtonPressed>()) {
                if (mb->button == sf::Mouse::Button::Left) {
                    sf::View currentView = camera->GetView();
                    Shoot::ShootVersSouris(shoot, *player, window, currentView);
                }
            }
        }

        // Score : chaque ennemi tué rapporte des points
        PvE::handleCollisionsJoueurShoot(shoot, m_ennemis, m_score);

        // Vies : décrément uniquement si le joueur n'est pas en invincibilité temporaire
        if (m_invincibleTimer > 0.f) {
            m_invincibleTimer -= dt;
        }
        else {
            int viesAvant = m_vies;
            PvE::handleCollisionsJoueurEnnemis(player, m_ennemis, m_vies);
            if (m_vies < viesAvant) {
                m_invincibleTimer = 1.0f; // 1 seconde d'invincibilité après un coup
            }
        }

        PvE::cleanupEnnemis(m_ennemis);

        hud.setScore(m_score);
        hud.setVies(m_vies);

        // Game over si le joueur n'a plus de vies
        if (m_vies <= 0) {
            isEnd = true;
        }

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

        playerPos = player->rect.getPosition();

        // Position de spawn des ennemis
        float spawnX = 1784.f;
        float spawnY = 475.f;
        float spawnXx = -229.f;
        float spawnYy = 488.f;  

        m_spawnerS.Update(dt, m_soldat, spawnX, spawnY);
        m_spawnerS2.Update(dt, m_soldat, spawnXx, spawnYy);

        AssignSoldatRoles(m_soldat, m_zoneManager, Team::Orange);
        AssignSoldatRoles(m_soldat, m_zoneManager, Team::Bleu);

        sf::Vector2f playerCenter = playerPos + sf::Vector2f(playerSize.x / 2.f, playerSize.y / 2.f);


        for (Soldat* t : m_soldat) {
            if (!t->alive) continue;

            SoldatBlackboard bb;
            bb.self = t;
            bb.allSoldats = &m_soldat;
            bb.zoneManager = &m_zoneManager;
            bb.dt = dt;
            bb.projectiles = &m_soldatProjectiles;
            bb.blocks = &m_battleWalls;

            m_soldatAI->Tick(bb);

            t->Update(dt, bb.moveTarget, &m_battleWalls);        // <- cache au lieu de GetBlocks()
            t->ResolveCollisionsSold(m_battleWalls);              // <- idem
        }
        // Mise à jour des projectiles et application des dégâts à l'impact
        for (SoldatProjectile* p : m_soldatProjectiles) {
            if (!p->alive) continue;
            p->Update(dt);

            for (Soldat* target : m_soldat) {
                if (!target->alive || target->GetTeam() == p->team) continue;
                sf::Vector2f d = target->rect.getPosition() - p->pos;
                float distSq = d.x * d.x + d.y * d.y;
                if (distSq <= 20.f * 20.f) { // rayon de collision du projectile
                    target->TakeDamage(p->damage);
                    p->alive = false;
                    break;
                }
            }
            // Le projectile s'arrête (disparaît) en touchant un mur
            if (p->alive) {
                for (Block* b : m_battleWalls) { // utilise le cache déjà proposé, sinon levels[currentLvl]->GetBlocks() filtré sur "MBlock"
                    sf::FloatRect projBounds(p->pos - sf::Vector2f(4.f, 4.f), sf::Vector2f(8.f, 8.f)); // ajuste selon la taille réelle du shape
                    if (projBounds.findIntersection(b->rect.getGlobalBounds())) {
                        p->alive = false;
                        break;
                    }
                }
            }
            // Projectile sorti de la carte = mort
            if (p->pos.x < -500.f || p->pos.x > 2200.f || p->pos.y < -500.f || p->pos.y > 2200.f) {
                p->alive = false;
            }
        }

        m_soldatProjectiles.erase(
            std::remove_if(m_soldatProjectiles.begin(), m_soldatProjectiles.end(),
                [](SoldatProjectile* p) {
                    if (!p->alive) { delete p; return true; }
                    return false;
                }),
            m_soldatProjectiles.end()
        );

        PvE::ResolveSoldatCollisions(m_soldat);
        m_zoneManager.Update(dt, m_soldat);
        PvE::cleanupSoldat(m_soldat);
        //PvE::cleanupSoldat(m_soldat);

        hud.setScore(m_score);
        hud.setVies(m_vies);

        // Décompte du timer de partie (mode Battle uniquement)
        m_battleTimer -= dt;
        if (m_battleTimer < 0.f) m_battleTimer = 0.f;

        // Game over si le joueur n'a plus de vies OU si le temps est écoulé
        if ((m_vies <= 0 || m_battleTimer <= 0.f) && !isEnd) {
            m_battleResultText = ComputeBattleWinner();   // <-- calcul une seule fois
            isEnd = true;
        }

        for (Shoot* s : shoot) {
            if (s != nullptr) {
                s->Update(dt, now);
            }
        }

        for (auto& ev : events) {
            if (auto* mb = ev.getIf<sf::Event::MouseButtonPressed>()) {
                if (mb->button == sf::Mouse::Button::Left) {
                    sf::View currentView = camera->GetView();
                    Shoot::ShootVersSouris(shoot, *player, window, currentView);
                }
            }
        }

        // Score : chaque ennemi tué rapporte des points
        PvE::handleCollisionsJoueurShootS(shoot, m_soldat, m_score);

        // Vies : décrément uniquement si le joueur n'est pas en invincibilité temporaire
        if (m_invincibleTimer > 0.f) {
            m_invincibleTimer -= dt;
        }
        else {
            int viesAvant = m_vies;
            PvE::handleCollisionsJoueurSoldat(player, m_soldat, m_vies);
            if (m_vies < viesAvant) {
                m_invincibleTimer = 1.0f; // 1 seconde d'invincibilité après un coup
            }
        }

        //PvE::cleanupSoldat(m_soldat);

        hud.setScore(m_score);
        hud.setVies(m_vies);

        // Game over si le joueur n'a plus de vies
        if (m_vies <= 0) {
            isEnd = true;
        }

        sf::Vector2f camCenter = camera->GetView().getCenter();
        parallax->Update(camCenter.x, camCenter.y);
        camera->Update(playerPos.x + playerSize.x / 2.f, playerPos.y + playerSize.y / 2.f, dt);

        gameTime = now;
        if (isEnd) state = END;
        break;
    }
    case END: {
        sf::View menuview = cameramenu->GetMenuView();
        letscontinue->UpdateHover(window, menuview);
        exit->UpdateHover(window, menuview);

        for (auto& ev : events) {
            if (auto* mb = ev.getIf<sf::Event::MouseButtonPressed>()) {
                if (mb->button == sf::Mouse::Button::Left) {
                    if (letscontinue->IsHovered()) {
                        sound.StopMusic();
                        isEnd = false;      // <-- essentiel : sinon retour immédiat à END
                        isRunning = true;

                        switch (m_gameMode) {
                        case GameMode::SURVIVAL:
                            SwitchToSurvival(levels);
                            player->rect.setPosition(sf::Vector2f(802.f, 926.f));
                            camera->SetZoom(1.25f);
                            camera->Update(802.f, 926.f, 0.f);
                            state = SURVIVALS;
                            break;

                        case GameMode::BATTLE:
                            SwitchToBattle(levels);
                            player->rect.setPosition(sf::Vector2f(1784.f, 475.f));
                            camera->SetZoom(1.44f);
                            camera->Update(1057.f, 768.f, 0.f);
                            state = BATTLES;
                            break;

                        case GameMode::TPS:
                        default:
                            SwitchToTPS(camera);
                            player->rect.setPosition(sf::Vector2f(690.f, 1178.f));
                            camera->Update(690.f, 1178.f, 0.f);
                            state = RUNNING;
                            break;
                        }
                    }
                    else if (exit->IsHovered()) {
                        sound.StopMusic();
                        isEnd = false;
                        SwitchToTPS(camera);
                        player->rect.setPosition(sf::Vector2f(690.f, 1178.f));
                        camera->Update(690.f, 1178.f, 0.f);
                        isRunning = false;
                        state = MENU;
                    }
                }
            }
        }
        break;
    }
    case MENU: {
        sf::View menuview = cameramenu->GetMenuView();
        start->UpdateHover(window, menuview);
        exit->UpdateHover(window, menuview);

        if (isRunning) {
            state = SELECT_PERSO;
            m_selectPerso.Reset();
        }
        break;
    }
    case PAUSE: {
        sf::View menuview = cameramenu->GetMenuView();
        m_pause->UpdateHover(window, menuview);

        for (auto& ev : events) {
            if (auto* mb = ev.getIf<sf::Event::MouseButtonPressed>()) {
                if (mb->button == sf::Mouse::Button::Left) {
                    if (m_pause->IsResumeHovered()) {
                        state = m_stateBeforePause;
                    }
                    else if (m_pause->IsQuitHovered()) {
                        sound.StopMusic();

                        // Réinitialise complètement l'état de jeu avant de retourner au menu,
                        // sinon le prochain lancement garde l'ancien mode (fond, niveau, blocks)
                        SwitchToTPS(camera);
                        player->rect.setPosition(sf::Vector2f(690.f, 1178.f));
                        camera->Update(690.f, 1178.f, 0.f);

                        isRunning = false;
                        state = MENU;
                    }
                }
            }
        }
        break;
    }
    case SELECT_PERSO: {
        sf::View menuview = cameramenu->GetMenuView();
        m_selectPerso.Update(window, menuview, events);
        if (m_selectPerso.IsConfirmed()) {
            static_cast<Joueur*>(player)->ChangeSpriteSheet(m_selectPerso.GetSelectedSpriteSheet());

            std::string facePath = m_selectPerso.GetSelectedFacePath();
            for (auto* npc : m_npcs) {
                npc->SetPlayerFace(facePath);
            }

            state = RUNNING;
        }
        break;
    }
    }
}

std::string Game::ComputeBattleWinner() {
    auto& zones = m_zoneManager.GetZones();
    if (zones.empty()) return "Match nul";

    float totalBleu = 0.f;
    for (const Zone& zone : zones) {
        totalBleu += std::clamp(zone.GetCapturePercent(Team::Bleu), 0.f, 100.f);
    }
    float avgBleu = totalBleu / static_cast<float>(zones.size());
    float avgOrange = 100.f - avgBleu;

    if (std::abs(avgBleu - avgOrange) < 0.01f) return "Match nul";
    return (avgBleu > avgOrange) ? "L'equipe Bleu a gagne !" : "L'equipe Orange a gagne !";
}