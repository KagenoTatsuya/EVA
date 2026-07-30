#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Entity.h"

class Button; // forward declaration, pas besoin d'inclure Button.h ici

class Input {
public:
    bool checkInteractionPressed();

    // Teste si un TouchBegan (de cet event ou de cette frame) tombe dans les
    // bornes du bouton. Indépendant de IsHovered()/UpdateHover() : le tactile
    // n'a pas de notion de survol, donc on teste directement les bornes,
    // comme le fait déjà le clic souris dans main.cpp.
    static bool IsButtonTouched(const sf::Event& event, Button* button);
    static bool IsButtonTouched(const std::vector<sf::Event>& events, Button* button);
};
#endif // INPUTMANAGER_H