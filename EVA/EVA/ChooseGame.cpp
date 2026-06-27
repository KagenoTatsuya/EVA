#include "ChooseGame.h"

ChooseGame::ChooseGame() {

	image = new sf::Texture("assets/pictures/EVA_arene.png ");
	sprite = new sf::Sprite(*image);
}

ChooseGame::ChooseGame(float screenW, float screenH) : screenW(screenW), screenH(screenH) {
	screenW = 1920.f;
	screenH = 1080.f;
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