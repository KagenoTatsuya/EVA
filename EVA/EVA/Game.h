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
#include "Pause.h"
#include "WallGrid.h"
#include "ProjectilePool.h"


class StartMenu;
class EndMenu;
class Pause;
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
    std::vector<Block*> m_ennemiWalls; // sous-liste des MBlock uniquement, pour le mode Battle
    WallGrid m_wallGrid;                // grille spatiale des murs, reconstruite dans SwitchToBattle
    Pause* m_pause = nullptr;
    ProjectilePool m_projectilePool;

    
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
        PAUSE,
    } state;

    NPC* m_activeNPC = nullptr;
    int m_vies = 3;
    int m_score = 0;
    float m_invincibleTimer = 0.f; // pour éviter de perdre 3 vies en une collision qui dure plusieurs frames
    State m_stateBeforePause = RUNNING;
    float m_battleTimer = 0.f;                        // temps restant en mode Battle
    static constexpr float BATTLE_DURATION = 300.f;   // 10 minutes = 600.f
    std::string m_battleResultText;

    // Effet sombre déclenché par un tir/collision Orange
    enum class DarknessPhase { None, FadeIn, Blinded, FadeOut } m_darknessPhase = DarknessPhase::None;
    float m_darknessPhaseTimer = 0.f; // temps écoulé dans la phase courante (FadeIn/FadeOut)
    static constexpr float kBattleDarknessRadius = 3000.f; // assez grand pour ne créer aucun assombrissement visible
    static constexpr float kHitDarknessRadius = 15.f;     // rayon "aveuglé"
    static constexpr float kHitDarknessFadeDuration = 1.f;      // durée de l'assombrissement ET de l'illumination
    static constexpr float kSpawnReturnRadius = 40.f;      // distance en dessous de laquelle le joueur est "revenu au spawn"
    static constexpr sf::Vector2f kBattleSpawnPos{ 1784.f, 475.f }; // cible de la flèche = spawn joueur en Battle
    void TriggerHitDarkness();
    void UpdateHitDarkness(float dt, sf::Vector2f playerPos);
    void RenderSpawnArrow(sf::RenderWindow& window, Camera* camera, Entity* player);

public:
    Game(sf::Vector2u windowSize);
    ~Game();
    // Switch vers le mode platformer (appelé quand le joueur passe la porte)
    void SwitchToSurvival(Camera* camera, std::vector<Level*>& levels);
    void SwitchToBattle(Camera* camera, std::vector<Level*>& levels);
    void SwitchToTPS(Camera* camera, std::vector<Level*>& levels);
    std::string ComputeBattleWinner();
    void Update(bool& isRunning, bool& isEnd, bool& isPause, float dt, float now,
        std::vector<sf::Event>& events, std::vector<Level*>& levels,
        sf::RenderWindow& window, Parallax* parallax, Camera* camera,
        CameraMenu* cameramenu, SoundManager& sound, Button* start,
        Button* exit, Button* letscontinue, Button* zombie, Button* arene, Entity* player, ChooseGame* choose, std::vector<Shoot*>& shoot);
    void Render(std::vector<Level*>& levels,
        sf::RenderWindow& window, Parallax* parallax, Camera* camera,
        CameraMenu* cameramenu, StartMenu* startmenu, EndMenu* endmenu,
        Button* start, Button* exit, Button* letscontinue, Button* zombie, Button* arene, Entity* player, ChooseGame* choose, std::vector<Shoot*>& shoot);
};