#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "DarknessEffect.h"
#include "NPC.h"
#include "Input.h"
#include "ChooseGame.h"
#include "Survival.h"
#include "Battle.h"
#include "Shoot.h"
#include "HUD.h"
#include "ZoneManager.h"
#include "SoldatAI.h"
#include "SelectPerso.h"

class StartMenu;
class EndMenu;
class Button;
class Level;
class Entity;
class Camera;
class CameraMenu;
class SoundManager;
class Shoot;
class Parallax;
class Block;
class DarknessEffect;
class NPC;
class Input;
class ChooseGame;
class Survival;
class HUD;
class SelectPerso;


class Game {
private:
    int newLvl;
    int currentLvl;
    float gameTime;
    sf::RenderTexture   m_sceneTexture;
    sf::Text*   m_hintText = nullptr;
    DarknessEffect      m_darkness;
    std::vector<PNJ*> m_pnjs;
    std::vector<NPC*>   m_npcs;
    EnnemiSpawner m_spawner;
    EnnemiSpawner m_spawner2;
    std::vector<Ennemi*> m_ennemis; 
    SoldatSpawnerB m_spawnerS;
    SoldatSpawnerO m_spawnerS2;
    std::vector<Soldat*> m_soldat;
    sf::Font            m_font;
    Input               m_input;
    sf::Texture* texture;
    sf::Sprite* bg;
    HUD hud;
    ZoneManager m_zoneManager;
    BTNodePtr   m_soldatAI = BuildSoldatBehaviorTree();
    SelectPerso m_selectPerso;
    std::vector<SoldatProjectile*> m_soldatProjectiles;
    std::vector<Block*> m_battleWalls; // sous-liste des MBlock uniquement, pour le mode Battle

    // Mode de jeu
    enum class GameMode {
        TPS,        // vue du dessus avec Joueur
        SURVIVAL,  // vue Survival avec Player2d
        BATTLE,
    } m_gameMode;

    enum State {
        MENU,
        RUNNING,
        SURVIVALS,
        BATTLES,
        END,
        SELECT_PERSO,
        CHOOSE_YESNO,   // discussion avec le NPC, attend O/N
        CHOOSE_MODE,    // écran ChooseGame avec boutons Zombie/Battle
    } state;

    NPC* m_activeNPC = nullptr;

    int m_vies = 3;
    int m_score = 0;
    float m_invincibleTimer = 0.f; // pour éviter de perdre 3 vies en une collision qui dure plusieurs frames

public:

    Game(sf::Vector2u windowSize);
    ~Game();

    // Switch vers le mode platformer (appelé quand le joueur passe la porte)
    void SwitchToSurvival(std::vector<Level*>& levels);
    void SwitchToBattle(std::vector<Level*>& levels);
    void SwitchToTPS(Camera* camera);

    void Update(bool& isRunning, bool& isEnd, float dt, float now,
        std::vector<sf::Event> events, std::vector<Level*>& levels,
        sf::RenderWindow& window, Parallax* parallax, Camera* camera,
        CameraMenu* cameramenu, SoundManager& sound, Button* start,
        Button* exit, Button* letscontinue, Button* zombie, Button* arene, Entity* player, ChooseGame* choose, std::vector<Shoot*>& shoot);

    void Render(std::vector<Level*>& levels,
        sf::RenderWindow& window, Parallax* parallax, Camera* camera,
        CameraMenu* cameramenu, StartMenu* startmenu, EndMenu* endmenu,
        Button* start, Button* exit, Button* letscontinue, Button* zombie, Button* arene, Entity* player, ChooseGame* choose, std::vector<Shoot*>& shoot);
};