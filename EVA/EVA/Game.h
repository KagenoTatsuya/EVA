#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "DarknessEffect.h"
#include "NPC.h"
#include "Input.h"
#include "Player2d.h"
#include "Tile.h"

class StartMenu;
class EndMenu;
class Button;
class Level;
class Entity;
class Camera;
class CameraMenu;
class SoundManager;
class Parallax;
class Block;
//class DarknessEffect;
class NPC;
class Input;

class Game {
    int newLvl;
    int currentLvl;
    float gameTime;
    sf::RenderTexture   m_sceneTexture;
    //DarknessEffect      m_darkness;
    std::vector<NPC*>   m_npcs;
    sf::Font            m_font;
    Input               m_input;
    Player2d* m_player2d;
    std::vector<Tile>   m_tiles;
    sf::Texture* texture;
    sf::Sprite* bg;

    // Mode de jeu
    enum class GameMode {
        TPS,        // vue du dessus avec Joueur
        PLATFORMER  // vue platformer avec Player2d
    } m_gameMode;

    // Caméra platformer
    sf::View m_platformerCamera;

    enum State {
        MENU,
        RUNNING,
        END,
    } state;

public:
    Game(sf::Vector2u windowSize);
    ~Game();

    // Switch vers le mode platformer (appelé quand le joueur passe la porte)
    void SwitchToPlatformer();
    void SwitchToTPS();

    void Update(bool& isRunning, bool& isEnd, float dt, float now,
        std::vector<sf::Event> events, std::vector<Level*>& levels,
        sf::RenderWindow& window, Parallax* parallax, Camera* camera,
        CameraMenu* cameramenu, SoundManager& sound, Button* start,
        Button* exit, Button* letscontinue, Entity* player);

    void Render(std::vector<Level*>& levels,
        sf::RenderWindow& window, Parallax* parallax, Camera* camera,
        CameraMenu* cameramenu, StartMenu* startmenu, EndMenu* endmenu,
        Button* start, Button* exit, Button* letscontinue, Entity* player);
};