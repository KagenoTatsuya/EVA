#include "Input.h"
#include "Button.h"

bool Input::checkInteractionPressed() {
    static bool wasPressed = false;
    bool isPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);
    if (isPressed && !wasPressed) {
        wasPressed = true;
        return true;
    }
    if (!isPressed) {
        wasPressed = false;
    }
    return false;
}

bool Input::IsButtonTouched(const sf::Event& ev, Button* button) {
    if (!button) return false;
    const auto* began = ev.getIf<sf::Event::TouchBegan>();
    if (!began) return false;

    float x = static_cast<float>(began->position.x);
    float y = static_cast<float>(began->position.y);

    return button->GetPosX() <= x && button->GetRightX() >= x &&
        button->GetPosY() <= y && button->GetBottomY() >= y;
}

bool Input::IsButtonTouched(const std::vector<sf::Event>& events, Button* button) {
    for (const auto& ev : events) {
        if (IsButtonTouched(ev, button)) return true;
    }
    return false;
}