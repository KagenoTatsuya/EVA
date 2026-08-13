#ifndef MENUSTART
#define MENUSTART

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>

class StartMenu {
private:
	sf::Texture* image;
	sf::Sprite* sprite;
	sf::Font* font;
	sf::Text* text;
	sf::Text* texte;
	float screenW, screenH;

public:

	StartMenu();
	StartMenu(float screenW, float screenH);

	void Render(sf::RenderWindow& window);

	~StartMenu();

};
#endif