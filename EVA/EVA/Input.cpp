#include "Input.h"

bool Input::checkInteractionPressed() {
    static bool wasPressed = false;

    bool isPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);

    if (isPressed && !wasPressed) {
        wasPressed = true;
        return true; // Interaction détectée
    }

    if (!isPressed) {
        wasPressed = false;
    }

    return false;
}