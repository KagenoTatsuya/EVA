#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "DarknessEffect.h"
#include "NPC.h"
#include "Input.h"
#include "ChooseGame.h"
#include "Survival.h"
#include "Shoot.h"

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

class Game {
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
    sf::Font            m_font;
    Input               m_input;
    sf::Texture* texture;
    sf::Sprite* bg;

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
        CHOOSE_YESNO,   // discussion avec le NPC, attend O/N
        CHOOSE_MODE,    // écran ChooseGame avec boutons Zombie/Battle
    } state;

    NPC* m_activeNPC = nullptr;

public:

    Game(sf::Vector2u windowSize);
    ~Game();

    // Switch vers le mode platformer (appelé quand le joueur passe la porte)
    void SwitchToSurvival(std::vector<Level*>& levels);
    void SwitchToBattle();
    void SwitchToTPS(Camera* camera);

    void Update(bool& isRunning, bool& isEnd, float dt, float now,
        std::vector<sf::Event> events, std::vector<Level*>& levels,
        sf::RenderWindow& window, Parallax* parallax, Camera* camera,
        CameraMenu* cameramenu, SoundManager& sound, Button* start,
        Button* exit, Button* letscontinue, Button* zombie, Button* battle, Entity* player, ChooseGame* choose, std::vector<Shoot*>& shoot);

    void Render(std::vector<Level*>& levels,
        sf::RenderWindow& window, Parallax* parallax, Camera* camera,
        CameraMenu* cameramenu, StartMenu* startmenu, EndMenu* endmenu,
        Button* start, Button* exit, Button* letscontinue, Button* zombie, Button* battle, Entity* player, ChooseGame* choose, std::vector<Shoot*>& shoot);
};