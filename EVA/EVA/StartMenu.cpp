#include "StartMenu.h"

StartMenu::StartMenu() : StartMenu(1920.f, 1080.f) {
}

StartMenu::StartMenu(float screenW, float screenH) : screenW(screenW), screenH(screenH) {
	image = new sf::Texture("assets/pictures/menus/Menu.png"); // espace en trop retiré
	sprite = new sf::Sprite(*image);

	font = new sf::Font("assets/fonts/arial.ttf");
	text = new sf::Text(*font, "Created by BROUILLARD Theo");
	texte = new sf::Text(*font, "B  O  R  D  E  A  U  X      L  A  C");
	text->setCharacterSize(24);
	text->setFillColor(sf::Color::White);
	text->setPosition({ screenW / 2.f + (-165), screenH - 40.f}); // screenW/H sont bien initialisés ici
	texte->setCharacterSize(42);
	texte->setFillColor(sf::Color::White);
	texte->setOutlineColor(sf::Color::White);
	texte->setOutlineThickness(0.75f);
	texte->setPosition({ screenW / 4.f + (-20), screenH / 2 + 170.f }); // screenW/H sont bien initialisés ici
}

void StartMenu::Render(sf::RenderWindow& window) {
	window.draw(*sprite);
	window.draw(*text);
	window.draw(*texte);
}

StartMenu::~StartMenu()
{
	delete sprite; sprite = nullptr;
	delete image;  image = nullptr;
	delete font;   font = nullptr;
	delete text;   text = nullptr;
	delete texte;   texte = nullptr;
}