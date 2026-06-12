#include "StartMenu.h"


StartMenu::StartMenu() {

	image = new sf::Texture("assets/pictures/menus/Menu.png ");
	sprite = new sf::Sprite(*image);
}

StartMenu::StartMenu(float screenW, float screenH) : screenW(screenW), screenH(screenH) {
	screenW = 1920.f;
	screenH = 1080.f;
}

void StartMenu::Render(sf::RenderWindow& window) {
	window.draw(*sprite);
}

StartMenu::~StartMenu()
{
	delete sprite;
	sprite = nullptr;
	delete image;
	image = nullptr;
}	