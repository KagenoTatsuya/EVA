#include "ChooseGame.h"

ChooseGame::ChooseGame() {

	image = new sf::Texture("assets/pictures/EVA_arene.png ");
	sprite = new sf::Sprite(*image);
}

void ChooseGame::Render(sf::RenderWindow& window) {
	window.draw(*sprite);
}

ChooseGame::~ChooseGame() {

	delete sprite;
	sprite = nullptr;
	delete image;
	image = nullptr;
}