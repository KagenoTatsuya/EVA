#include <iostream>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Input.h"
#include "Camera.h"
#include "Parallax.h"
#include "CameraMenu.h"
#include "Button.h"
#include "EndMenu.h"
#include "Game.h"
#include "SoundManager.h"
#include "StartMenu.h"
#include "Level.h"

#define LEVEL_NUMBER 2

int main() {

    // Récupère la résolution native de l'écran
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    // Création de la fenêtre en plein écran
    sf::RenderWindow window(desktopMode, " EVA ", sf::Style::Default);

    // Juste après la création de la fenêtre
    if (!sf::Shader::isAvailable()) {
        std::cerr << "Shaders non supportes !" << std::endl;
        return -1;
    }
    std::cout << "Shaders OK" << std::endl;

    // Limite le framerate pour éviter une utilisation CPU/GPU excessive
    window.setFramerateLimit(60);

    Game game(window.getSize());
    Input input;
    std::vector<Level*> levels;
    levels.push_back(new Level("Level1.txt"));
    for (int i = 1; i < LEVEL_NUMBER; i++) {
        levels.push_back(nullptr);
    }
    for (int i = 1; i < levels.size(); i++) {
        if (levels[i] == nullptr) {
            levels[i] = new Level(levels[i - 1]->GetNextLevel());
        }
    }

    StartMenu* startmenu = new StartMenu();
    EndMenu* endmenu = new EndMenu();
    Button* letscontinue = new Continue();
    Button* exit = new Exit();
    Button* start = new Start();
    SoundManager* soundManager = new SoundManager();
    Parallax* parallax = new Parallax(1920, 1080.f);
    Camera* camera = new Camera(1600.0f, 800.0f, 800.f, 1280.f);
    CameraMenu* cameramenu = new CameraMenu(1920.0f, 1080.0f);

    //Chargement des images pour le parallax
    parallax->Load("assets/pictures/EVA_maps.png");

    // Création du joueur au centre de l'écran
    Joueur* player = new Joueur(683.f, 1178.f);

    bool isRunning = false;
    bool isSettings = false;
    bool endSim = false;
    bool isPause = false;
    sf::Clock clock;        // Horloge pour deltaTime
    float now = 0.0f;
    float gameTime = 0.0f;
    float dt = 0.0f;
    std::vector<sf::Event> events;

    // Boucle principale
    while (window.isOpen()) {
        dt = clock.restart().asSeconds();
        now += dt;

        // Process events
        events.clear();
        while (const auto event = window.pollEvent()) {

            // Close window: exit
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (!isRunning) {
                    if (start->GetPosX() <= static_cast<float>(mousePos.x) && start->GetRightX() >= static_cast<float>(mousePos.x) &&
                        start->GetPosY() <= static_cast<float>(mousePos.y) && start->GetBottomY() >= static_cast<float>(mousePos.y)) {
                        isRunning = true;
                    }
                    else if (exit->GetPosX() <= static_cast<float>(mousePos.x) && exit->GetRightX() >= static_cast<float>(mousePos.x) &&
                        exit->GetPosY() <= static_cast<float>(mousePos.y) && exit->GetBottomY() >= static_cast<float>(mousePos.y)) {
                        window.close();
                    }
                }
                if (endSim) {
                    if (letscontinue->GetPosX() <= static_cast<float>(mousePos.x) && start->GetRightX() >= static_cast<float>(mousePos.x) &&
                        letscontinue->GetPosY() <= static_cast<float>(mousePos.y) && start->GetBottomY() >= static_cast<float>(mousePos.y)) {
                        endSim = false;
                        isRunning = false;
                    }
                    else if (exit->GetPosX() <= static_cast<float>(mousePos.x) && exit->GetRightX() >= static_cast<float>(mousePos.x) &&
                        exit->GetPosY() <= static_cast<float>(mousePos.y) && exit->GetBottomY() >= static_cast<float>(mousePos.y)) {
                        endSim = false;
                        isRunning = false;
                    }
                }
            }
            events.push_back(*event);
        }
        // Update du joueur
        //player->Update(dt, window.getSize());

        // Efface l'écran avec une couleur
        window.clear(sf::Color::Black);

        game.Update(isRunning, endSim, /*isPause,*/ dt, now,
            events, levels, window, parallax, camera, cameramenu, *soundManager,
            start, exit, letscontinue, player);

        // Draw the sprite
        game.Render(levels, window, parallax, camera, cameramenu, startmenu, endmenu,/* pause,*/
            start, exit, letscontinue, player);

        //player->Render(&window);

        // Update the window
        window.display();
    }
    events.clear();

    delete player; player = nullptr;
    delete soundManager; soundManager = nullptr;
    delete cameramenu; cameramenu = nullptr;
    delete camera; camera = nullptr;
    delete parallax; parallax = nullptr;
    delete startmenu; startmenu = nullptr;
    delete start; start = nullptr;
    delete exit; exit = nullptr;
    delete letscontinue; letscontinue = nullptr;
    delete endmenu; endmenu = nullptr;

    return 0;
}