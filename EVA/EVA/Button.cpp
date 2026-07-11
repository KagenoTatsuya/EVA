#include "Button.h"


Button::Button() {
}

void Button::SetText(std::string textStirng, float posx, float posy, float width, float height, sf::Font& font) {
    text.setFont(font);
    text.setString(textStirng);
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(20);
    float centerX = (GetPosX() + GetRightX()) / 2.f;
    float centerY = (GetPosY() + GetBottomY()) / 2.f;

    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.getCenter());
    text.setPosition(sf::Vector2f(posx + width / 2.f, posy + height / 2.f));
}

void Button::LoadTextures(const std::string& normalPath, const std::string& hoverPath) {
    if (!textureNormal.loadFromFile(normalPath))
        std::cerr << "Erreur texture normale: " << normalPath << std::endl;
    if (!textureHover.loadFromFile(hoverPath))
        std::cerr << "Erreur texture normale: " << hoverPath << std::endl;

    // Appliquer la texture normale par défaut
    button.setTexture(&textureNormal);
}


void Button::UpdateHover(sf::RenderWindow& window, sf::View& menuview) {
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(mousePixel, menuview);

    sf::FloatRect bounds(sf::Vector2f(GetPosX(), GetPosY()),
        sf::Vector2f(GetRightX() - GetPosX(), GetBottomY() - GetPosY()));
    bool wasHovered = isHovered;
    isHovered = bounds.contains(mousePos);

    if (isHovered && !wasHovered)  OnHover();
    if (!isHovered && wasHovered)  OnNormal();
}


Exit::Exit() {
    posx = 1305.f;
    posy = 780.f;
    width = 180.f;
    height = 160.f;
    if (!font.openFromFile("assets/fonts/arial.ttf"))
        std::cerr << "Can't find the font file" << std::endl;

    button.setPosition(sf::Vector2f(posx, posy));
    button.setSize(sf::Vector2f(width, height));
    //button.setFillColor(sf::Color::Red);

    LoadTextures("assets/pictures/menus/Button_exit.png", "assets/pictures/menus/Button_exit2.png");

    SetText("", posx, posy, width, height, font);
}

void Exit::Render(sf::RenderWindow& window) {
    window.draw(button);
    window.draw(text);
}

void Exit::OnHover() {
    button.setTexture(&textureHover);
}

void Exit::OnNormal() {
    button.setTexture(&textureNormal);
}


Start::Start() {
    posx = 405.f;
    posy = 720.f;
    width = 280.f;
    height = 270.f;
    if (!font.openFromFile("assets/fonts/arial.ttf"))
        std::cerr << "Can't find the font file" << std::endl;

    button.setPosition(sf::Vector2f(posx, posy));
    button.setSize(sf::Vector2f(width, height));
    //button.setFillColor(sf::Color::Green);

    LoadTextures("assets/pictures/menus/Button_start.png", "assets/pictures/menus/Button_start2.png");

    SetText("", posx, posy, width, height, font);
}

void Start::Render(sf::RenderWindow& window) {
    window.draw(button);
    window.draw(text);
}

void Start::OnHover() {
    button.setTexture(&textureHover);
}

void Start::OnNormal() {
    button.setTexture(&textureNormal);
}

Continue::Continue() {
    posx = 905.f;
    posy = 740.f;
    width = 120.f;
    height = 70.f;
    if (!font.openFromFile("assets/fonts/arial.ttf"))
        std::cerr << "Can't find the font file" << std::endl;

    button.setPosition(sf::Vector2f(posx, posy));
    button.setSize(sf::Vector2f(width, height));
    button.setFillColor(sf::Color::Green);

    LoadTextures("assets/pictures/menus/Button_continue.png", "assets/pictures/menus/Button_continue2.png");

    SetText("Continue", posx, posy, width, height, font);
}

void Continue::Render(sf::RenderWindow& window) {
    window.draw(button);
    window.draw(text);
}

void Continue::OnHover() {
    button.setTexture(&textureHover);
}

void Continue::OnNormal() {
    button.setTexture(&textureNormal);
}


Zombie::Zombie() {
    posx = 405.f;
    posy = 540.f;
    width = 300.f;
    height = 300.f;
    if (!font.openFromFile("assets/fonts/arial.ttf"))
        std::cerr << "Can't find the font file" << std::endl;

    button.setPosition(sf::Vector2f(posx, posy));
    button.setSize(sf::Vector2f(width, height));
    //button.setFillColor(sf::Color::Magenta);

    LoadTextures("assets/pictures/menus/Button_zombi.png", "assets/pictures/menus/Button_zombi2.png");

    SetText("", posx, posy, width, height, font);
}

void Zombie::Render(sf::RenderWindow& window) {
    window.draw(button);
    window.draw(text);
}

void Zombie::OnHover() {
    button.setTexture(&textureHover);
}

void Zombie::OnNormal() {
    button.setTexture(&textureNormal);
}


Arene::Arene() {
    posx = 1305.f;
    posy = 540.f;
    width = 300.f;
    height = 300.f;
    if (!font.openFromFile("assets/fonts/arial.ttf"))
        std::cerr << "Can't find the font file" << std::endl;

    button.setPosition(sf::Vector2f(posx, posy));
    button.setSize(sf::Vector2f(width, height));
    //button.setFillColor(sf::Color::Blue);

    LoadTextures("assets/pictures/menus/Button_arene.png", "assets/pictures/menus/Button_arene2.png");

    SetText("", posx, posy, width, height, font);
}

void Arene::Render(sf::RenderWindow& window) {
    window.draw(button);
    window.draw(text);
}

void Arene::OnHover() {
    button.setTexture(&textureHover);
}

void Arene::OnNormal() {
    button.setTexture(&textureNormal);
}
