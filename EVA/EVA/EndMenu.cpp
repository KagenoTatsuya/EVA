#include "EndMenu.h"


EndMenu::EndMenu() {

	image = new sf::Texture("assets/pictures/menus/End.png ");
	sprite = new sf::Sprite(*image);
}

void EndMenu::Render(sf::RenderWindow& window) {
	window.draw(*sprite);
}

EndMenu::~EndMenu() {

	delete sprite;
	sprite = nullptr;
	delete image;
	image = nullptr;
}